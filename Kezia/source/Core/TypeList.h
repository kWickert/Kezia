#pragma once

#include "Core/Any.h"

#include <vector>

namespace Kezia
{
	template<typename T>
	struct TypeTraits
	{
		typedef T BaseType; 
	};

	template<typename T>
	struct TypeTraits<const T>
	{
		typedef T BaseType;
	};

	template<typename T>
	struct TypeTraits<T &>
	{
		typedef T BaseType;
	};

	template<typename T>
	struct TypeTraits<const T &>
	{
		typedef T BaseType;
	};

	template<class T, class U>
	struct TypeList
	{
		typedef T Head;
		typedef U Tail;
	};

	class NullType
	{};

	#define TYPELIST_1(T1) TypeList<T1, NullType>
	#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2)>
	#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3)>

	template <class TList> struct Length;
	template <> struct Length<NullType>
	{
		enum { value = 0 };
	};

	template <class T, class U>
	struct Length<TypeList<T, U> >
	{
		enum { value = 1 + Length<U>::value };
	};

	template <class TList, Kezia::U32 i> struct TypeAt;

	template<class Head, class Tail>
	struct TypeAt<TypeList<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template<class Head, class Tail, Kezia::U32 i>
	struct TypeAt<TypeList<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};

	template <class TList, Kezia::U32 i> struct GenerateFunction;

	#define FN_RET_VAL typename TypeAt<TList, 0>::Result
	#define FN_ARG_LIST_1 typename TypeAt<TList, 1>::Result
	#define FN_ARG_LIST_2 FN_ARG_LIST_1, typename TypeAt<TList, 2>::Result
	#define FN_ARG_LIST_3 FN_ARG_LIST_2, typename TypeAt<TList, 3>::Result
	#define FN_ARG_LIST_4 FN_ARG_LIST_3, typename TypeAt<TList, 4>::Result
	#define FN_ARG_LIST_5 FN_ARG_LIST_4, typename TypeAt<TList, 5>::Result

	template <class TList>
	struct GenerateFunction<TList, 1>
	{
		typedef FN_RET_VAL (*Result)();
	};

	template <class TList>
	struct GenerateFunction<TList, 2>
	{
		typedef FN_RET_VAL (*Result)(FN_ARG_LIST_1);
	};

	template <class TList>
	struct GenerateFunction<TList, 3>
	{
		typedef FN_RET_VAL (*Result)(FN_ARG_LIST_2);
	};

	template <class TList>
	struct GenerateFunction<TList, 4>
	{
		typedef FN_RET_VAL (*Result)(FN_ARG_LIST_3);
	};

	template <class TList, U32 i> struct AnyExtractor;

	#define ADD_ANY(i) { typename TypeTraits<TypeAt<TList, i - 1>::Result >::BaseType any; stream >> any; anyList.push_back(Any(any)); }

	#define ADD_ALL_1 ADD_ANY(1)
	#define ADD_ALL_2 ADD_ALL_1 ADD_ANY(2) 
	#define ADD_ALL_3 ADD_ALL_2 ADD_ANY(3) 
	#define ADD_ALL_4 ADD_ALL_3 ADD_ANY(4) 
	#define ADD_ALL_5 ADD_ALL_4 ADD_ANY(5) 

	template <class TList>
	struct AnyExtractor<TList, 0>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			return anyList;
		}
	};

	template <class TList>
	struct AnyExtractor<TList, 1>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			anyList.reserve(1);

			ADD_ALL_1

			return anyList;
		}
	};

	template <class TList>
	struct AnyExtractor<TList, 2>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			anyList.reserve(2);

			ADD_ALL_2

			return anyList;
		}
	};

	template <class TList>
	struct AnyExtractor<TList, 3>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			anyList.reserve(3);

			ADD_ALL_3

			return anyList;
		}
	};

	template <class TList>
	struct AnyExtractor<TList, 4>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			anyList.reserve(4);

			ADD_ALL_4

			return anyList;
		}
	};

	template <class TList>
	struct AnyExtractor<TList, 5>
	{
		static std::vector<Any> Parse(std::stringstream & stream)
		{
			std::vector<Any> anyList;
			anyList.reserve(5);

			ADD_ALL_5

			return anyList;
		}
	};

	template <class TList>
	struct ToFunction
	{
		typedef typename GenerateFunction<TList, Length<TList>::value>::Result Result;
	};

	template <typename R, typename Fn, Kezia::U32 i> struct Executor;

	template <typename Fn>  
	struct Executor<void, Fn, 0>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			function();
			return Kezia::Any();
		}
	};

	template <typename R, typename Fn>  
	struct Executor<R, Fn, 0>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			return function();
		}
	};

	template <typename Fn>  
	struct Executor<void, Fn, 1>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			function(args[0]);
			return Kezia::Any();
		}
	};

	template <typename R, typename Fn>  
	struct Executor<R, Fn, 1>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			return function(args[0]);
		}
	};

	template <typename Fn>  
	struct Executor<void, Fn, 2>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			function(args[0], args[1]);
			return Kezia::Any();
		}
	};

	template <typename R, typename Fn>  
	struct Executor<R, Fn, 2>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			return function(args[0], args[1]);
		}
	};

	template <typename Fn>  
	struct Executor<void, Fn, 3>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			function(args[0], args[1], args[2]);
			return Kezia::Any();
		}
	};

	template <typename R, typename Fn>  
	struct Executor<R, Fn, 3>
	{
		static Kezia::Any Run(Fn function, const std::vector<Kezia::Any> & args)
		{
			return function(args[0], args[1], args[2]);
		}
	};

	template<class TList> struct TypeCheck;

	template <class Head, class Tail>
	struct TypeCheck<TypeList<Head, Tail> >
	{
		static bool Check(const std::vector<Kezia::Any> & v, Kezia::U8 offset = 0)
		{
			if(v[offset].TypeInfo() != typeid(Head))
				return false;

			return TypeCheck<Tail>::Check(v, offset + 1);
		}
	};

	template <class Head>
	struct TypeCheck<TypeList<Head, NullType> >
	{
		static bool Check(const std::vector<Kezia::Any> & v, Kezia::U8 offset = 0)
		{
			return v[offset].TypeInfo() == typeid(Head);
		}
	};

	template<> struct TypeCheck<NullType>
	{
		static bool Check(const std::vector<Kezia::Any> & v, Kezia::U8 offset = 0)
		{
			return true;
		}
	};
}