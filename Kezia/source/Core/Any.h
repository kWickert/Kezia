#pragma once

#include <algorithm>
#include <typeinfo>

namespace Kezia
{
	class Any
	{
	public:
		Any()
			:	m_Content(nullptr)
		{}

		Any(const Any & other)
			:	m_Content(other.m_Content ? other.m_Content->Clone() : nullptr)
		{}

		template<typename T>
		Any(const T & value)
			:	m_Content(new Holder<T>(value))
		{}

		Any & Swap(Any & rhs)
		{
			std::swap(m_Content, rhs.m_Content);
			return *this;
		}

		Any & operator=(const Any & rhs)
		{
			return Swap(Any(rhs));
		}

		template<typename T>
		Any & operator=(const T & rhs)
		{
			return Swap(Any(rhs));
		}

		~Any()
		{
			delete m_Content;
		}

		operator const void *() const
		{
			return m_Content;
		}

		template<typename T>
		operator T() const
		{
			return AnyCast<T>(*this);
		}

		template<typename T>
		bool CopyTo(T & value) const
		{
			const T * copyable = ToPointer<T>() : if(copyable) value = *copyable;

			return copyable;
		}

		template<typename T>
		const T * ToPointer() const
		{
			return TypeInfo() == typeid(T) ? &static_cast< Holder<T> *>(m_Content)->Held : nullptr;
		}

		const std::type_info & TypeInfo() const
		{
			return m_Content ? m_Content->TypeInfo() : typeid(void);
		}

	private:
		class Placeholder
		{
		public:
			virtual ~Placeholder()
			{}

			virtual const std::type_info & TypeInfo() const = 0;

			virtual Placeholder * Clone() const = 0;
		};

		template<typename T>
		class Holder : public Placeholder
		{
		public:
			Holder(const T & value)
				:	Held(value)
			{}

			virtual const std::type_info & TypeInfo() const
			{
				return typeid(T);
			}

			virtual Placeholder * Clone() const
			{
				return new Holder(Held);
			}

			const T Held;
		};

		Placeholder * m_Content;
	};

	template<typename T>
	T AnyCast(const Any & operand)
	{
		const T * result = operand.ToPointer<T>();
		return result ? *result : throw std::bad_cast();
	}
}