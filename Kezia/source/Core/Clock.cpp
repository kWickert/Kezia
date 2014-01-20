#include "Clock.h"

#include "Core/Time.h"

#include "Event/EventManager.h"

namespace Kezia
{
	Clock::Clock(Clock * parent)
		:	m_Parent(parent),
			m_ElapsedSeconds(0.0),
			m_LastUpdateSeconds(0.0),
			m_MaxDeltaSeconds(20.0),
			m_IsPaused(false),
			m_TimeScale(1.0)
			
	{
		if(m_Parent != nullptr)
			m_Parent->m_Children.push_back(this);
	}

	Clock::~Clock()
	{
		for(auto child = m_Children.begin(); child != m_Children.end(); ++child)
		{
			Clock * c = (*child);
			c->m_Parent = m_Parent;

			m_Parent->m_Children.push_back(c);
		}
	}

	void Clock::AdvanceClock(F64 deltaSeconds)
	{
		m_LastUpdateSeconds = GetLiveTimeSeconds();

		if(deltaSeconds > m_MaxDeltaSeconds)
			deltaSeconds = m_MaxDeltaSeconds;

		if(m_IsPaused)
			deltaSeconds = 0.0;
		else
			deltaSeconds *= m_TimeScale;

		m_ElapsedSeconds += deltaSeconds;

		while(!m_ScheduledEvents.empty() && m_ScheduledEvents.front().scheduledTime <= m_ElapsedSeconds)
		{
			ScheduledEvent & e = m_ScheduledEvents.front();
			g_EventManager->FireEvent(e.eventName, e.eventProperties);

			if(e.loopPeriod > 0)
			{
				InsertNewEvent(m_ElapsedSeconds + e.loopPeriod, e.loopPeriod, e.eventName, e.eventProperties);
			}

			m_ScheduledEvents.erase(m_ScheduledEvents.begin());
		}

		/*
		if(!m_ScheduledEvents.empty() && m_ScheduledEvents.front().scheduledTime <= m_ElapsedSeconds)
		{
			auto fireEnd(m_ScheduledEvents.begin());

			while(fireEnd != m_ScheduledEvents.end() && fireEnd->scheduledTime <= m_ElapsedSeconds)
				++fireEnd;

			for(auto it = m_ScheduledEvents.begin(); it != fireEnd; ++it)
			{
				g_EventManager->FireEvent(it->eventName, it->eventProperties);
			}

			m_ScheduledEvents.erase(m_ScheduledEvents.begin(), fireEnd);
		}	
		*/

		for(auto child = m_Children.begin(); child != m_Children.end(); ++child)
		{
			Clock * c = (*child);
			c->AdvanceClock(deltaSeconds);
		}
	}

	void Clock::ScheduleFixedEventFire(const F64 absoluteTimeSeconds, const std::string & eventName, const NamedProperties & properties)
	{
		InsertNewEvent(absoluteTimeSeconds, 0, eventName, properties);
	}

	void Clock::ScheduleDelayedEventFire(F64 delayedTimeSeconds, const std::string & eventName, const bool shouldLoop, const NamedProperties & properties)
	{
		InsertNewEvent(m_ElapsedSeconds + delayedTimeSeconds, shouldLoop ? delayedTimeSeconds : 0, eventName, properties);
	}

	void Clock::CancelEventFire(const std::string & eventName)
	{
		for(auto it = m_ScheduledEvents.begin(); it != m_ScheduledEvents.end(); )
		{
			if((*it).eventName == eventName)
			{
				it = m_ScheduledEvents.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	bool Clock::IsPaused() const
	{
		return m_IsPaused;
	}

	void Clock::Pause()
	{
		m_IsPaused = true;
	}

	void Clock::Unpause()
	{
		m_IsPaused = false;
	}

	void Clock::SetMaxTimeStepSeconds(const F64 maxStepSeconds)
	{
		m_MaxDeltaSeconds = maxStepSeconds;
	}

	const F64 Clock::GetTimeScale() const
	{
		return m_TimeScale;
	}

	void Clock::SetTimeScale(const F64 timeScale)
	{
		m_TimeScale = timeScale;
	}

	const F64 Clock::GetElapsedSeconds() const
	{
		return m_ElapsedSeconds;
	}

	void Clock::SetElapsedSeconds(const F64 elapsedSeconds)
	{
		m_ElapsedSeconds = elapsedSeconds;
	}

	void Clock::InsertNewEvent(const F64 absoluteTimeSeconds, const F64 loopDelay, const std::string & eventName, const NamedProperties & properties)
	{
		ScheduledEvent newEvent(absoluteTimeSeconds, loopDelay, eventName, properties);

		if(m_ScheduledEvents.empty())
		{
			m_ScheduledEvents.push_back(newEvent);
		}
		else
		{
			auto insertPosition = m_ScheduledEvents.begin();
			while(insertPosition != m_ScheduledEvents.end() && insertPosition->scheduledTime < newEvent.scheduledTime)
				++insertPosition;

			m_ScheduledEvents.insert(insertPosition, newEvent);
		}
	}
}