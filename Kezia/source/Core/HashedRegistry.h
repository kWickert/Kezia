#pragma once

#include "Core/Types.h"
#include "SuperFastHash.h"

#include <algorithm>
#include <map>

namespace Kezia
{
	typedef const U32 (*HashFn)(const std::string &);

	struct HashedKey
	{
		HashedKey(U32 keyValue, const std::string & key = "")
			:	keyValue(keyValue),
				key(key)
		{}
				

		bool operator<(const HashedKey & other) const
		{
			return keyValue < other.keyValue;
		}

		const std::string key;
		const U32 keyValue;
	};

	template<typename Value_T>
	class HashedRegistry
	{
	public:
		/*	KEZIA: The registry can be configured to use different hashing functions,
			case insensitive hash and case sensitive hash are both available as static
			functions on this class */
		HashedRegistry(HashFn hashFunction = CaseSensitiveHash)
			:	m_HashFunction(hashFunction)
		{}

		HashedRegistry(const HashedRegistry & other)
			:	m_Registry(other.m_Registry),
				m_HashFunction(other.m_HashFunction)
		{}

		~HashedRegistry()
		{}

		HashedRegistry & operator=(const HashedRegistry & rhs) const
		{
			HashedRegistry tmp(rhs);
			swap(*this, tmp);
			return *this;
		}

		typedef typename std::map<HashedKey, Value_T>::iterator Iterator;
		typedef typename std::map<HashedKey, Value_T>::const_iterator Const_Iterator;

		/*	KEZIA: returns and iterator to the element at key
			and a boolean - true if a new element was inserted or false if the key already existed */
		std::pair<Iterator, bool> Insert(const std::string & key, const Value_T & value)
		{
			return m_Registry.insert(std::make_pair(HashedKey(m_HashFunction(key), key), value));
		}

		Iterator Find(const std::string & key)
		{
			return m_Registry.find(HashedKey(m_HashFunction(key)));
		}

		Const_Iterator Find(const std::string & key) const
		{
			return m_Registry.find(HashedKey(m_HashFunction(key)));
		}

		void Erase(Const_Iterator it)
		{
			m_Registry.erase(it);
		}

		Iterator Begin()
		{
			return m_Registry.begin();
		}

		Iterator End()
		{
			return m_Registry.end();
		}

		Const_Iterator Begin() const
		{
			return m_Registry.begin();
		}

		Const_Iterator End() const
		{
			return m_Registry.end();
		}

		const U32 Size() const
		{
			return m_Registry.size();
		}

		HashFn GetHashFunction() const
		{
			return m_HashFunction;
		}

		static const U32 CaseSensitiveHash(const std::string & key)
		{
			return SuperFastHash(key);
		}

		static const U32 CaseInsensitiveHash(const std::string & key)
		{
			std::string lowerKey;
			lowerKey.resize(key.size());
			std::transform(key.begin(), key.end(), lowerKey.begin(),
				[&](const char & c)->char
			{
				return tolower(c);	
			});

			return SuperFastHash(lowerKey);
		}

		friend void swap(HashedRegistry<Value_T> & lhs, HashedRegistry<Value_T> & rhs)
		{
			std::swap(lhs.m_Registry, rhs.m_Registry);
			std::swap(lhs.m_HashFunction, rhs.m_HashFunction);
		}
	private:
		std::map<HashedKey, Value_T> m_Registry;

		HashFn m_HashFunction;
	};
}