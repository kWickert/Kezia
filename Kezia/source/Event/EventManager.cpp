#include "EventManager.h"

namespace Kezia
{
	ListenerBase::~ListenerBase()
	{}

	EventManager::EventManager()
	{}

	EventManager::~EventManager()
	{
		for(auto outer = m_EventMap.Begin(); outer != m_EventMap.End(); ++outer)
		{
			std::vector<ListenerBase *> & listeners = outer->second;

			for(auto inner = listeners.begin(); inner != listeners.end(); ++inner)
			{
				delete *inner;
			}
		}
	}

	void EventManager::FireEvent(const std::string & eventName, NamedProperties & arguments)
	{
		auto findResult = m_EventMap.Find(eventName);
		if(findResult != m_EventMap.End())
		{
			for(auto listener = findResult->second.begin(); listener != findResult->second.end(); ++listener)
			{
				(*listener)->Invoke(arguments);
			}
		}
	}
}