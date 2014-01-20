#include "Event.h"

#include "Core/Clock.h"

#include "Event/EventManager.h"

namespace Kezia
{
	void FireEvent(const std::string & eventName, NamedProperties & args)
	{
		g_EventManager->FireEvent(eventName, args);
	}

	void FireEvent(const std::string & eventName)
	{
		NamedProperties args;
		g_EventManager->FireEvent(eventName, args);
	}

	void ScheduleFixedEventFire(const F64 absoluteTimeSeconds, const std::string & eventName, const NamedProperties & properties)
	{
		g_MasterClock->ScheduleFixedEventFire(absoluteTimeSeconds, eventName, properties);
	}

	void ScheduleDelayedEventFire(const F64 delayedTimeSeconds, const std::string & eventName, const bool shouldLoop, const NamedProperties & properties)
	{
		g_MasterClock->ScheduleDelayedEventFire(delayedTimeSeconds, eventName, shouldLoop, properties);
	}

	void CancelEventFire(const std::string & eventName)
	{
		g_MasterClock->CancelEventFire(eventName);
	}
}