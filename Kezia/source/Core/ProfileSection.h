#pragma once

#include "Core/Types.h"

#include "Profiler.h"
#include "Time.h"

namespace Kezia
{
	class ProfileSection
	{
	public:
		ProfileSection(const U32 tagId)
			:	m_TagId(tagId),
				m_StartTimeSeconds(GetLiveTimeSeconds())
		{}

		~ProfileSection()
		{
			g_Profiler->ReportNewInstance(m_TagId, GetLiveTimeSeconds() - m_StartTimeSeconds);
		}
	private:
		const U32 m_TagId;
		const F64 m_StartTimeSeconds;
	};
}