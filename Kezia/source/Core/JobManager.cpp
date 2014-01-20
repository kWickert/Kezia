#include "JobManager.h"

#include "Job.h"

#include "Core/Assert.h"

#include "Event/Event.h"

namespace Kezia
{
	void WorkerThreadMain(void * data)
	{
		JobManager * manager = (JobManager *)data;
		const tthread::thread::id myId = tthread::this_thread::get_id();

		for(;;)
		{
			if(manager->ShouldWorkerDie(myId))
				break;

			Job * myJob = manager->ClaimNextJob(myId);

			if(myJob != nullptr)
			{
				myJob->Execute();
				manager->PostCompletedJob(myJob, myId);
			}
		}

		manager->PostWorkerDeath(myId);
	}

	JobManager::JobManager(const U32 workerCount)
		:	m_RemovedCount(0),
			m_OwnerThreadId(tthread::this_thread::get_id())
	{
		for(U32 i = 0; i < workerCount; i++)
			CreateWorkerThread();
	}

	void JobManager::Update()
	{
		KeziaAssert(tthread::this_thread::get_id() == m_OwnerThreadId);

		std::list<Job *> completedJobs;

		//copy the list of completed jobs
		//and clear so others can post to it
		{
			tthread::lock_guard<tthread::mutex> lock(m_CompletedJobMutex);
			completedJobs = m_CompletedJobs;
			m_CompletedJobs.clear();
		}


		for(auto completedJob = completedJobs.begin(); completedJob != completedJobs.end(); ++completedJob)
		{
			m_RemovedCount++;

			Job * j = *completedJob;

			CallbackEventInfo info = ExtractCallbackEventInfo(j);

			FireEvent(info.eventName, info.eventArguments);

			delete j;
		}	

		if(GetWorkerCount() == 0 && GetUnclaimedJobCount() > 0)
		{
			tthread::thread::id mainThreadId = tthread::this_thread::get_id();

			Job * nextJob = m_UnclaimedJobs.front();//ClaimNextJob(mainThreadId);
			m_UnclaimedJobs.pop_front();

			nextJob->Execute();

			CallbackEventInfo info = ExtractCallbackEventInfo(nextJob);
			
			FireEvent(info.eventName, info.eventArguments);

			delete nextJob;
		}
	}

	const U32 JobManager::GetWorkerCount()
	{
		U32 workerCount = 0;
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);
			workerCount = m_WorkerInfoMap.size();
		}

		return workerCount;
	}

	const U32 JobManager::GetUnclaimedJobCount()
	{
		U32 jobCount = 0;
		{
			tthread::lock_guard<tthread::mutex> lock(m_UnclaimedJobMutex);

			jobCount = m_UnclaimedJobs.size();
		}

		return jobCount;
	}

	const U32 JobManager::GetInProgressJobCount()
	{
		U32 jobCount = 0;
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);

			for(auto worker = m_WorkerInfoMap.begin(); worker != m_WorkerInfoMap.end(); ++worker)
			{
				if(worker->second.currentJob != nullptr)
					jobCount++;
			}
		}

		return jobCount;
	}

	void JobManager::SubmitNewJob(Job * newJob, const std::string & onJobCompleteEventName, NamedProperties eventArguments)
	{
		{
			tthread::lock_guard<tthread::mutex> lock(m_UnclaimedJobMutex);
			
			auto job = m_UnclaimedJobs.begin();
			for(; job != m_UnclaimedJobs.end(); ++job)
			{
				Job * j = *job;
				if(newJob->GetPriority() > j->GetPriority())
					break;
			}

			m_UnclaimedJobs.insert(job, newJob);
		}

		{
			tthread::lock_guard<tthread::mutex> lock(m_CallbackEventMutex);

			KeziaAssert(m_CallbackEventMap.find(newJob) == m_CallbackEventMap.end());

			m_CallbackEventMap.insert(std::make_pair(newJob, CallbackEventInfo(onJobCompleteEventName, eventArguments)));
		}
	}

	void JobManager::CreateWorkerThread()
	{
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);

			WorkerInfo info;
			info.workerThread = new tthread::thread(WorkerThreadMain, this);

			m_WorkerInfoMap.insert(std::make_pair(info.workerThread->get_id(), info));
		}
	}
	
	Job * JobManager::ClaimNextJob(const tthread::thread::id & workerId)
	{
		Job * nextJob = nullptr;

		//get the next job
		{
			tthread::lock_guard<tthread::mutex> lock(m_UnclaimedJobMutex);
			if(!m_UnclaimedJobs.empty())
			{
				nextJob = m_UnclaimedJobs.front();
				m_UnclaimedJobs.pop_front();
			}
		}

		//update the corresponding worker info
		if(nextJob != nullptr)
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);

			auto findResult = m_WorkerInfoMap.find(workerId);

			KeziaAssert(findResult != m_WorkerInfoMap.end());

			findResult->second.currentJob = nextJob;
		}

		return nextJob;
	}

	void JobManager::PostCompletedJob(Job * job, const tthread::thread::id & workerId)
	{
		//get the job that was just finished
		//and update the corresponding workerinfo
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);

			auto findResult = m_WorkerInfoMap.find(workerId);

			KeziaAssert(findResult != m_WorkerInfoMap.end());

			WorkerInfo & info = findResult->second;
			info.currentJob = nullptr;
		}

		//post the completed job
		{
			tthread::lock_guard<tthread::mutex> lock(m_CompletedJobMutex);
			m_CompletedJobs.push_back(job);
		}
	}

	JobManager::CallbackEventInfo JobManager::ExtractCallbackEventInfo(Job * job)
	{
		CallbackEventInfo info;
		{
			tthread::lock_guard<tthread::mutex> lock(m_CallbackEventMutex);

			auto findResult = m_CallbackEventMap.find(job);
			KeziaAssert(findResult != m_CallbackEventMap.end());

			info = findResult->second;

			m_CallbackEventMap.erase(findResult);
		}

		return info;
	}

	void JobManager::RequestWorkerDeath(const tthread::thread::id & workerId)
	{
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);
			m_WorkerInfoMap[workerId].shouldDie = true;
		}
	}

	bool JobManager::ShouldWorkerDie(const tthread::thread::id & workerId)
	{
		bool shouldDie = false;

		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);
			shouldDie = m_WorkerInfoMap[workerId].shouldDie;
		}

		return shouldDie;
	}

	void JobManager::PostWorkerDeath(const tthread::thread::id & workerId)
	{
		{
			tthread::lock_guard<tthread::mutex> lock(m_WorkerInfoMutex);
			m_WorkerInfoMap.erase(workerId);
		}
	}
}