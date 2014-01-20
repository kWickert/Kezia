#pragma once

#include "Core/Types.h"

#include "Event/NamedProperties.h"

#include <list>
#include <map>

#include <tinythread.h>

namespace Kezia
{
	class Job;
	class JobManager;

	void WorkerThreadMain(void * data);

	class JobManager
	{
	public:
		JobManager(const U32 workerCount = 0);
		~JobManager();

		void Update();

		const U32 GetWorkerCount();
		const U32 GetUnclaimedJobCount();
		const U32 GetInProgressJobCount();

		void SubmitNewJob(Job * newJob, const std::string & onJobCompleteEventName, NamedProperties  eventArguments);

		friend void WorkerThreadMain(void * data);
	private:
		struct CallbackEventInfo;

		void CreateWorkerThread();

		Job * ClaimNextJob(const tthread::thread::id & workerId);
		void PostCompletedJob(Job * job, const tthread::thread::id & workerId);

		CallbackEventInfo ExtractCallbackEventInfo(Job * job);

		void RequestWorkerDeath(const tthread::thread::id & workerId);
		bool ShouldWorkerDie(const tthread::thread::id & workerId);
		void PostWorkerDeath(const tthread::thread::id & workerId);

		struct WorkerInfo
		{
			WorkerInfo()
				:	currentJob(nullptr),
					workerThread(nullptr),
					shouldDie(false)
			{}

			Job * currentJob;
			tthread::thread * workerThread;

			bool shouldDie;
		};

		struct CallbackEventInfo
		{
			CallbackEventInfo()
				:	eventName(),
					eventArguments()
			{}

			CallbackEventInfo(const std::string & eventName, const NamedProperties & eventArguments)
				:	eventName(eventName),
					eventArguments(eventArguments)
			{}

			std::string eventName;
			NamedProperties eventArguments;
		};

		U32 m_RemovedCount;

		tthread::thread::id m_OwnerThreadId;

		std::map<Job *, CallbackEventInfo> m_CallbackEventMap;
		tthread::mutex m_CallbackEventMutex;

		std::map<tthread::thread::id, WorkerInfo> m_WorkerInfoMap;
		tthread::mutex m_WorkerInfoMutex;

		std::list<Job *> m_UnclaimedJobs;
		tthread::mutex m_UnclaimedJobMutex;

		std::list<Job *> m_CompletedJobs;
		tthread::mutex m_CompletedJobMutex;
	};
}