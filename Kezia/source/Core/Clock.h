#pragma once

#include "Core/Types.h"

#include "Core/HashedString.h"
#include "Event/NamedProperties.h"

#include <vector>

namespace Kezia
{
	struct ScheduledEvent
	{
		ScheduledEvent(F64 scheduledTime, F64 loopPeriod, const std::string & eventName, const NamedProperties & properties)
			:	scheduledTime(scheduledTime),
				eventName(eventName),
				eventProperties(properties),
				loopPeriod(loopPeriod)
		{}

		~ScheduledEvent()
		{}

		F64 scheduledTime;
		F64 loopPeriod;
		std::string eventName;
		NamedProperties eventProperties;
	};

	class Clock
	{
	public:
		Clock(Clock * parent = nullptr);
		~Clock();

		void AdvanceClock(F64 deltaSeconds);

		void ScheduleFixedEventFire(const F64 absoluteTimeSeconds, const std::string & eventName, const NamedProperties & properties = NamedProperties());
		void ScheduleDelayedEventFire(const F64 delayedTimeSeconds, const std::string & eventName, const bool shouldLoop = false, const NamedProperties & properties = NamedProperties());

		void CancelEventFire(const std::string & eventName);

		bool IsPaused() const;
		void Pause();
		void Unpause();

		void SetMaxTimeStepSeconds(const F64 maxStepSeconds);

		const F64 GetTimeScale() const;
		void SetTimeScale(const F64 timeScale);

		const F64 GetElapsedSeconds() const;
		void SetElapsedSeconds(const F64 elapsedSeconds = 0.0);
	private:
		void InsertNewEvent(const F64 absoluteTimeSeconds, const F64 loopDelay, const std::string & eventName, const NamedProperties & properties);

		F64 m_ElapsedSeconds;
		F64 m_TimeScale;

		F64 m_LastUpdateSeconds;
		F64 m_MaxDeltaSeconds;

		bool m_IsPaused;

		Clock * m_Parent;
		std::vector<Clock *> m_Children;

		std::vector<ScheduledEvent> m_ScheduledEvents;
	};

	extern Clock * g_MasterClock;
}