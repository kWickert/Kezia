#pragma once

#include "Core/Assert.h"
#include "Core/NonCopyable.h"

namespace Kezia
{
	template<class Singleton_T>
	class Singleton : NonCopyable
	{
	public:
		static void CreateSingleton()
		{
			k_Singleton = new Singleton_T();
		}

		static Singleton_T * GetSingleton()
		{
			KeziaAssert(k_Singleton != nullptr);

			return k_Singleton;
		}

		static bool IsInitialized()
		{
			return k_Singleton == nullptr;
		}
	private:
		static Singleton_T * k_Singleton;
	};

	template<typename Singleton_T>
	Singleton_T * Singleton<Singleton_T>::k_Singleton = nullptr;

}