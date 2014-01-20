#pragma once

#include "Core/Assert.h"
#include "Core/HashedString.h"
#include "Core/Types.h"

#include "Time.h"

#include <map>
#include <vector>

namespace Kezia
{
	typedef U32 ProfileTagId;

	class Profiler
	{
	public:
		struct ProfileData;
		class ProfileDataIterator;

	public:
		Profiler()
			:	m_NextAvailableId(0)
		{}
		
		~Profiler()
		{}

		inline const ProfileTagId GetTagId(const HashedString & label)
		{
			const U32 hash = label.GetHashValue();
			auto findResult = m_TagMap.find(hash);

			if(findResult == m_TagMap.end())
			{
				m_TagMap.insert(std::make_pair(hash, m_NextAvailableId));
				m_ProfileDataMap.insert(std::make_pair(m_NextAvailableId, ProfileData(label.GetString())));

				m_NextAvailableId++;
			}

			return m_TagMap.find(hash)->second;
		}

		/*
		inline void EnableSnapshots(const std::string & label)
		{
			EnableSnapshots(GetTagId(label));
		}

		inline void EnableSnapshots(const ProfileTagId tagId)
		{
			KeziaAssert(m_ProfileDataMap.find(tagId) != m_ProfileDataMap.end());

			m_ProfileDataMap[tagId].isRecording = true;
		}

		inline void DisableSnapshots(const std::string & label)
		{
			DisableSnapshots(GetTagId(label));
		}

		inline void DisableSnapshots(const ProfileTagId tagId)
		{
			KeziaAssert(m_ProfileDataMap.find(tagId) != m_ProfileDataMap.end());

			m_ProfileDataMap[tagId].isRecording = false;
		}
		*/

		inline void ReportNewInstance(const ProfileTagId tagId, const F64 elapsedTime)
		{
			auto findResult = m_ProfileDataMap.find(tagId);

			if(findResult == m_ProfileDataMap.end())
				return;

			ProfileData & data = findResult->second;
			data.hitCount++;
			data.totalFrameDuration += elapsedTime;
			
			if(elapsedTime > data.longestInstanceThisFrame)
			{
				data.longestInstanceThisFrame = elapsedTime;
			}

			if(data.isRecording)
			{
				ProfileData::FrameSnapshot snapshot;
				snapshot.frameDuration = elapsedTime;
				snapshot.liveTime = GetLiveTimeSeconds();

				data.snapshots.push_back(snapshot);
			}
		}

		inline void AdvanceFrame()
		{
			for(auto profileDataIter = m_ProfileDataMap.begin(); profileDataIter != m_ProfileDataMap.end(); ++profileDataIter)
			{
				ProfileData & profileData = profileDataIter->second;
				
				if(profileData.longestInstanceThisFrame > profileData.longestInstanceThisSession)
				{
					profileData.longestInstanceThisSession = profileData.longestInstanceThisFrame;
				}

				profileData.averageFrameDuration = 0.1 * profileData.totalFrameDuration + 0.9 * profileData.averageFrameDuration;

				profileData.longestInstanceThisFrame = 0.0;
				profileData.totalFrameDuration = 0.0;
			}
		}

		inline ProfileDataIterator Begin() const
		{
			return ProfileDataIterator(this);
		}

		inline ProfileDataIterator End() const
		{
			return ProfileDataIterator(this, m_NextAvailableId);
		}

		inline ProfileDataIterator Find(const HashedString & label)
		{
			auto tagFindResult = m_TagMap.find(label.GetHashValue());
			
			if(tagFindResult == m_TagMap.end())
				return End();

			const ProfileTagId tagId = tagFindResult->second;

			auto dataFindResult = m_ProfileDataMap.find(tagId);
			
			if(dataFindResult == m_ProfileDataMap.end())
				return End();
			else
				return ProfileDataIterator(this, tagId);
		}

		struct ProfileData
		{
			ProfileData(const std::string & tagLabel)
				:	tagLabel(tagLabel),
					hitCount(0),
					averageFrameDuration(0),
					totalFrameDuration(0),
					longestInstanceThisFrame(0),
					longestInstanceThisSession(0),
					isRecording(false)
			{}

			struct FrameSnapshot
			{
				FrameSnapshot()
					:	frameDuration(0),
						liveTime(0)
				{}

				F64 frameDuration;
				F64 liveTime;
			};

			const std::string tagLabel;

			U32 hitCount;
			F64 totalFrameDuration;

			F64 averageFrameDuration;

			F64 longestInstanceThisFrame;
			F64 longestInstanceThisSession;

			bool isRecording;
			std::vector<FrameSnapshot> snapshots;
		};

		class ProfileDataIterator
		{
		public:
			ProfileDataIterator(const Profiler * profiler)
				:	m_Profiler(profiler),
					m_CurrentTagId(0)
			{}

			ProfileDataIterator(const Profiler *profiler, const ProfileTagId tagId)
				:	m_Profiler(profiler),
					m_CurrentTagId(tagId)
			{}

			bool operator==(const ProfileDataIterator & other) const
			{
				return m_CurrentTagId == other.m_CurrentTagId;
			}

			bool operator!=(const ProfileDataIterator & other) const
			{
				return !operator==(other);
			}

			const ProfileData & operator*() const
			{
				return m_Profiler->GetProfileData(m_CurrentTagId);
			}

			void operator++()
			{
				if(m_CurrentTagId < m_Profiler->m_NextAvailableId)
					++m_CurrentTagId;
			}

			void operator++(int)
			{
				if(m_CurrentTagId < m_Profiler->m_NextAvailableId)
					m_CurrentTagId++;
			}

			void operator--()
			{
				if(m_CurrentTagId > 0)
					m_CurrentTagId--;
			}

			void operator--(int)
			{
				if(m_CurrentTagId > 0)
					--m_CurrentTagId;
			}
		private:
			U32 m_CurrentTagId;

			const Profiler * m_Profiler;
		};
	private:
		const ProfileData & GetProfileData(const ProfileTagId tagId) const
		{
			KeziaAssert(tagId < m_NextAvailableId);

			auto findResult = m_ProfileDataMap.find(tagId);

			return findResult->second;
		}

		/*
		struct TagNode
		{
			TagNode(TagNode * parent, const std::string & tagName, const U32 tagId)
				:	parent(parent),
					tagName(tagName),
					tagId(tagId)
			{
				parent->children.push_back(this);
			}

			TagNode * parent;
			std::vector<TagNode *> children;

			std::string tagName;
			const U32 tagId;
		};
		*/

		U32 m_NextAvailableId;

		std::map<const U32, const ProfileTagId> m_TagMap;

		std::map<const ProfileTagId, ProfileData> m_ProfileDataMap;
	};

	extern Profiler * g_Profiler;
}