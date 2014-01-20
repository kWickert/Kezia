#pragma once

#include "Core/Types.h"
#include "Core/HashedString.h"
#include "Core/HashedRegistry.h"

#include "Core/Logger.h"

#include <map>
#include <iostream>

namespace Kezia
{
	//TODO deep copy
	#pragma region TypedData
	class TypedDataBase
	{
	public:
		virtual ~TypedDataBase();

		virtual TypedDataBase * Clone() = 0;

		virtual const type_info & GetTypeInfo() const = 0;
	};

	template<typename Value_T>
	class TypedData : public TypedDataBase
	{
	public:
		TypedData(const Value_T & value);
		virtual ~TypedData();

		virtual TypedDataBase * Clone();

		virtual const type_info & GetTypeInfo() const;

		inline void SetValue(const Value_T & value);
		inline void GetValue(Value_T & outValue);
	private:
		Value_T m_Value;
	};

	template<typename Value_T>
	TypedData<Value_T>::TypedData(const Value_T & value)
		:	m_Value(value)
	{}

	template<typename Value_T>
	TypedData<Value_T>::~TypedData()
	{}

	template<typename Value_T>
	TypedDataBase * TypedData<Value_T>::Clone()
	{
		return new TypedData<Value_T>(m_Value);
	}

	template<typename Value_T>
	const type_info & TypedData<Value_T>::GetTypeInfo() const
	{
		return typeid(Value_T);
	}

	template<typename Value_T>
	inline void TypedData<Value_T>::SetValue(const Value_T & value)
	{
		m_Value = value;
	}

	template<typename Value_T>
	inline void TypedData<Value_T>::GetValue(Value_T & outValue)
	{
		outValue = m_Value;
	}
	#pragma endregion

	class NamedProperties
	{
	public:
		NamedProperties();
		NamedProperties(const NamedProperties & other);
		~NamedProperties();

		NamedProperties & operator=(const NamedProperties & rhs);

		template<typename Value_T>
		bool GetProperty(const std::string & name, Value_T & outValue);

		template<typename Value_T>
		void SetProperty(const std::string & name, const Value_T & value);

		friend void swap(NamedProperties & lhs, NamedProperties & rhs);
	private:
		HashedRegistry<TypedDataBase *> m_Properties;
	};

	void swap(NamedProperties & lhs, NamedProperties & rhs);

	template<typename Value_T>
	bool NamedProperties::GetProperty(const std::string & name, Value_T & outValue)
	{
		bool success = false;

		auto findResult = m_Properties.Find(name);

		if(findResult != m_Properties.End())
		{
			if(findResult->second->GetTypeInfo() == typeid(Value_T))
			{
				reinterpret_cast< TypedData<Value_T>* >(findResult->second)->GetValue(outValue);

				success = true;
			}
		}

		if(!success)
		{
			LOG("Could not get value " << name);
		}

		return success;
	}

	template<typename Value_T>
	void NamedProperties::SetProperty(const std::string & name, const Value_T & value)
	{
		auto findResult = m_Properties.Find(name);
		if(findResult == m_Properties.End())
		{
			m_Properties.Insert(name, new TypedData<Value_T>(value));
		}
		else
		{
			if(findResult->second->GetTypeInfo() == typeid(Value_T))
			{
				reinterpret_cast< TypedData<Value_T> *>(findResult->second)->SetValue(value);
			}
		}
	}
}