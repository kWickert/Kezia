#pragma once

#include "Core/Types.h"
#include "Core/WindowsInclude.h"

#include "Math/MathUtility.h"

namespace Kezia
{
	template<typename Value_T, U32 BufferSize>
	class CircularBuffer
	{
	public:
		CircularBuffer()
			:	m_First(0),
				m_Size(0)
		{
			ZeroMemory(m_Data, BufferSize * sizeof(Value_T));
		}

		~CircularBuffer()
		{}

		const U32 Size() const
		{
			return m_Size;
		}

		void Push(const Value_T & value)
		{
			U32 lastIndex = PreviousIndex(m_First);

			if(IsMaxSize())
			{
				m_First = NextIndex(m_First);
			}
			else
			{
				m_Size = Min(m_Size + 1, BufferSize);
			}

			m_Data[NextIndex(lastIndex)] = value;
		}

		class ConstIterator
		{
		public:
			ConstIterator()
				:	m_Index(0),
					m_Buffer(nullptr)
			{}

			ConstIterator operator++()
			{
				if(m_Buffer != nullptr)
				{
					m_Index = m_Buffer->NextIndex(m_Index);
				}

				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator tmp(*this);

				if(m_Buffer != nullptr)
				{
					m_Index = m_Buffer->NextIndex(m_Index);
				}

				return tmp;
			}

			ConstIterator operator--()
			{
				if(m_Buffer != nullptr)
				{
					m_Index = m_Buffer->PreviousIndex(m_Index);
				}

				return *this;
			}

			ConstIterator operator--(int)
			{
				ConstIterator tmp(*this);

				if(m_Buffer != nullptr)
				{
					m_Index = m_Buffer->PreviousIndex(m_Index);
				}

				return tmp;
			}

			bool operator==(const ConstIterator & rhs) const
			{
				return m_Index == rhs.m_Index;
			}

			bool operator!=(const ConstIterator & rhs) const
			{
				return !(*this == rhs);
			}

			const Value_T operator*() const
			{
				return m_Buffer->m_Data[m_Index];
			}

			friend class CircularBuffer<Value_T, BufferSize>;
		private:
			ConstIterator(const U32 index, const CircularBuffer * buffer)
				:	m_Index(index),
					m_Buffer(buffer)
			{}

			U32 m_Index;
			const CircularBuffer<Value_T, BufferSize> * m_Buffer;
		};

		ConstIterator Begin() const
		{
			return ConstIterator(m_First, this);
		}

		friend class ConstIterator;
	private:
		U32 m_First;
		U32 m_Size;

		U32 NextIndex(U32 index) const
		{
			return index >= (m_Size - 1) ? 0 : index + 1;
		}

		U32 PreviousIndex(U32 index) const
		{
			return index == 0 ? m_Size - 1 : index - 1;
		}

		bool IsMaxSize() const
		{
			return m_Size == BufferSize;
		}

		Value_T m_Data[BufferSize];
	};
}