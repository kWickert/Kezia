#pragma once

#include "Event/NamedProperties.h"

#include <string>

namespace Kezia
{
	void FireEvent(const std::string & eventName, NamedProperties & args);
	void FireEvent(const std::string & eventName);

	void ScheduleFixedEventFire(const F64 absoluteTimeSeconds, const std::string & eventName, const NamedProperties & properties = NamedProperties());
	void ScheduleDelayedEventFire(const F64 delayedTimeSeconds, const std::string & eventName, const bool shouldLoop = false, const NamedProperties & properties = NamedProperties());

	void CancelEventFire(const std::string & eventName);
}