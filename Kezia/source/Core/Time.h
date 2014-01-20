#pragma once

#include "Core/Types.h"

#include "Core/WindowsInclude.h"

#include <sstream>
#include <time.h>

namespace Kezia
{
	inline F64 GetFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return static_cast<F64>(frequency.QuadPart);
	}

	inline F64 GetLiveTimeSeconds()
	{
		static F64 inverseFrequency = 1.0 / GetFrequency();

		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);

		return time.QuadPart * inverseFrequency;
	}

	class TimeStamp
	{
	public:
		TimeStamp()
		{
			static time_t rawTime;

			time(&rawTime);
			m_TimeStamp = rawTime;

			static SYSTEMTIME sTime;

			GetSystemTime(&sTime);
			m_Millisecond = sTime.wMilliseconds;
		}

		const std::string GetGlobalTime() const
		{
			struct tm timeInfo;

			timeInfo = *gmtime(&m_TimeStamp);

			return asctime(&timeInfo);
		}

		const std::string GetLocalTime() const
		{
			struct tm timeInfo;

			timeInfo = *localtime(&m_TimeStamp);

			return asctime(&timeInfo);
		}

		const F64 DifferenceSeconds(const TimeStamp & rhs) const
		{
			F64 seconds;

			seconds = difftime(m_TimeStamp, rhs.m_TimeStamp);

			seconds += (static_cast<F64>(m_Millisecond) - static_cast<F64>(rhs.m_Millisecond)) * 0.001;

			return seconds;
		}

		bool operator>(const TimeStamp & rhs) const
		{
			return DifferenceSeconds(rhs) > 0.0;
		}

	private:
		time_t m_TimeStamp;
		U16 m_Millisecond;
	};
} 