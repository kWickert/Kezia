#pragma once

#include "NamedProperties.h"

#include "Core/HashedRegistry.h"
#include "Core/Types.h"
 
#include <map>
#include <vector>

namespace Kezia
{
	#pragma region Listener
	class ListenerBase
	{
	public:
		virtual ~ListenerBase();
		virtual void Invoke(NamedProperties & arguments) = 0;
	};

	template<typename Object_T>
	class ObjectListener : public ListenerBase
	{
	private:
		typedef void(Object_T::*Function)(NamedProperties &);

	public:
		ObjectListener(Object_T * object, Function function);
		~ObjectListener();

		virtual void Invoke(NamedProperties & arguments);

		Object_T * GetObject();
		Function GetFunction();
	private:
		Object_T * m_Object;
		Function m_Function;
	};

	template<typename Object_T>
	ObjectListener<Object_T>::ObjectListener(Object_T * object, Function function)
		:	m_Object(object),
			m_Function(function)
	{}

	template<typename Object_T>
	ObjectListener<Object_T>::~ObjectListener()
	{}

	template<typename Object_T>
	void ObjectListener<Object_T>::Invoke(NamedProperties & arguments)
	{
		(m_Object->*m_Function)(arguments);
	}

	template<typename Object_T>
	Object_T * ObjectListener<Object_T>::GetObject()
	{
		return m_Object;
	}

	template<typename Object_T>
	typename ObjectListener<Object_T>::Function ObjectListener<Object_T>::GetFunction()
	{
		return m_Function;
	}
	#pragma endregion

	class EventManager
	{
	public:
		EventManager();
		~EventManager();

		void FireEvent(const std::string & eventName, NamedProperties & arguments);

		template<typename Object_T, typename Function_T>
		void RegisterObjectForEvent(const std::string & eventName, Object_T & object, Function_T function);

		template<typename Object_T>
		void UnregisterObjectForEvent(const std::string & eventName, Object_T & object);

		template<typename Object_T, typename Function_T>
		void UnregisterObjectForEvent(const std::string & eventName, Object_T & object, Function_T function);

		template<typename Object_T>
		void UnregisterObjectForAllEvents(const Object_T & object);

	private:
		HashedRegistry< std::vector<ListenerBase *> > m_EventMap;
	};

	extern EventManager * g_EventManager;

	template<typename Object_T>
	void EventManager::UnregisterObjectForAllEvents(const Object_T & object)
	{
		for(auto eventPair = m_EventMap.Begin(); eventPair != m_EventMap.End(); ++eventPair)
		{
			std::vector<ListenerBase *> & listeners = eventPair->second;

			for(U32 i = 0; i < listeners.size(); ++i)
			{
				//if the listener wraps object, remove it
				ObjectListener<Object_T> * cast = dynamic_cast<ObjectListener<Object_T> *>(listeners[i]);
				if(cast && cast->GetObject() == &object)
				{
					listeners[i--] = eventPair->second.back();
					eventPair->second.pop_back();
				}
			}
		}
	}

	template<typename Object_T, typename Function_T>
	void EventManager::UnregisterObjectForEvent(const std::string & eventName, Object_T & object, Function_T function)
	{
		auto findResult = m_EventMap.Find(eventName);

		std::vector<ListenerBase *> & listeners = findResult->second;

		for(U32 i = 0; i < listeners.size(); ++i)
		{
			//if the listener wraps object, remove it
			ObjectListener<Object_T> * cast = dynamic_cast<ObjectListener<Object_T> *>(listeners[i]);
			if(cast && cast->GetObject() == &object && cast->GetFunction() == function)
			{
				listeners[i--] = find->second.back();
				find->second.pop_back();
			}
		}
	}

	template<typename Object_T>
	void EventManager::UnregisterObjectForEvent(const std::string & eventName, Object_T & object)
	{
		auto findResult = m_EventMap.find(eventName);

		std::vector<ListenerBase *> & listeners = findResult->second;

		for(U32 i = 0; i < listeners.size(); ++i)
		{
			//if the listener wraps object, remove it
			ObjectListener<Object_T> * cast = dynamic_cast<ObjectListener<Object_T> *>(listeners[i]);
			if(cast && cast->GetObject() == &object)
			{
				listeners[i--] = find->second.back();
				find->second.pop_back();
			}
		}
	}

	template<typename Object_T, typename Function_T>
	void EventManager::RegisterObjectForEvent(const std::string & eventName, Object_T & object, Function_T function)
	{
		auto findResult = m_EventMap.Find(eventName);	
		if(findResult != m_EventMap.End())
		{
			bool alreadyRegistered = false;
			for(auto listener = findResult->second.begin(); listener != findResult->second.end(); ++listener)
			{
				//if the listener wraps object and function, remove it
				ObjectListener<Object_T> * cast = dynamic_cast<ObjectListener<Object_T> *>(*listener);

				//is it already registered?
				if(cast && cast->GetObject() == &object && cast->GetFunction() == function)
				{
					alreadyRegistered = true;
					break;
				}
			}

			if(!alreadyRegistered)
				findResult->second.push_back(new ObjectListener<Object_T>(&object, function));
		}
		else
		{
			auto insertResult = m_EventMap.Insert(eventName, std::vector<ListenerBase *>());
			insertResult.first->second.push_back(new ObjectListener<Object_T>(&object, function));
		}
	}

}