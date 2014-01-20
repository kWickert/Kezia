#pragma once

#include "Core/Assert.h"
#include "Core/HashedString.h"
#include "Core/Types.h"

#include <map>

namespace Kezia
{
	template<typename Registrant_T>
	class Registry
	{
	public:
		Registry()
		{}

		~Registry()
		{}

		static Registrant_T & Create(const HashedString & name)
		{
			return *k_Registry.insert(std::make_pair(name.GetHashValue(), new Registrant_T())).first->second;
		}

		template<typename T>
		static Registrant_T & Create(const HashedString & name, const T & arg)
		{
			return *k_Registry.insert(std::make_pair(name.GetHashValue(), new Registrant_T(arg))).first->second;
		}

		template<typename T1, typename T2>
		static Registrant_T & Create(const HashedString & name, const T1 & arg1, const T2 & arg2)
		{
			return *k_Registry.insert(std::make_pair(name.GetHashValue(), new Registrant_T(arg1, arg2))).first->second;
		}

		template<typename T1, typename T2, typename T3>
		static Registrant_T & Create(const HashedString & name, const T1 & arg1, const T2 & arg2, const T3 & arg3)
		{
			return *k_Registry.insert(std::make_pair(name.GetHashValue(), new Registrant_T(arg1, arg2, arg3))).first->second;
		}

		template<typename T1, typename T2, typename T3, typename T4>
		static Registrant_T & Create(const HashedString & name, const T1 & arg1, const T2 & arg2, const T3 & arg3, const T4 & arg4)
		{
			return *k_Registry.insert(std::make_pair(name.GetHashValue(), new Registrant_T(arg1, arg2, arg3, arg4))).first->second;
		}

		static Registrant_T & Get(const HashedString & name)
		{
			auto findResult = k_Registry.find(name.GetHashValue());
			KeziaAssert(findResult != k_Registry.end());

			return *findResult->second;
		}

		static bool Exists(const HashedString & name)
		{
			auto findResult = k_Registry.find(name.GetHashValue());

			return findResult != k_Registry.end();
		}

		static void Delete(const HashedString & name)
		{
			auto findResult = k_Registry.find(name.GetHashValue());
			KeziaAssert(findResult != k_Registry.end());

			delete findResult->second;

			k_Registry.erase(findResult);
		}

	private:
		static std::map<U32, Registrant_T *> k_Registry; 
	};

	template<typename Registrant_T>
	std::map<U32, Registrant_T *> Registry<Registrant_T>::k_Registry = std::map<U32, Registrant_T *>();
}