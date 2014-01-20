#pragma once

#include "Core/Types.h"
#include "Math/MathUtility.h"

namespace Kezia
{
	class PoolAllocator
	{
	public:
		PoolAllocator();
		~PoolAllocator();

		Byte * Allocate(const S32 desiredSizeBytes, const U8 tag = 0, const char * file = nullptr, int line = 0);
		void Deallocate(Byte * pointer);

		const U32 GetPoolSize() const;

		const U32 GetAllocatedBytes() const;
		const U32 GetAllocationCount() const;

		const U32 GetAverageAllocationSizeBytes() const;

		void ReportMemoryLeaks();

		struct MemoryBlockMetaData
		{
			MemoryBlockMetaData(const S32 sizeBytes, MemoryBlockMetaData * next = nullptr, MemoryBlockMetaData * previous = nullptr, const U8 tag = 0, const char * file = nullptr, U32 line = 0)
				:	sizeBytes(-sizeBytes),
					next(next),
					previous(previous),
					tag(tag),
					file(file),
					line(line)
			{}

			inline bool IsFree() const 
			{
				return sizeBytes < 0;
			}

			inline const U32 GetSize() const
			{
				return AbsoluteValue(sizeBytes);
			}

			MemoryBlockMetaData * next;
			MemoryBlockMetaData * previous;

			U8 tag;

			const char * file;
			U32 line;

			S32 sizeBytes;
		};

		class MemoryBlockIterator
		{
		public:
			MemoryBlockIterator(MemoryBlockMetaData * metaData)
				:	m_CurrentMetaData(metaData)
			{}

			MemoryBlockIterator(const MemoryBlockIterator & other)
				:	m_CurrentMetaData(other.m_CurrentMetaData)
			{}

			~MemoryBlockIterator()
			{}

			MemoryBlockIterator operator++()
			{
				m_CurrentMetaData = m_CurrentMetaData->next;
				return *this;
			}

			MemoryBlockIterator operator++(int)
			{
				MemoryBlockIterator previous(*this);
				++(*this);

				return previous;
			}

			bool operator==(const MemoryBlockIterator & other) const
			{
				return m_CurrentMetaData == other.m_CurrentMetaData;
			}

			bool operator!=(const MemoryBlockIterator & other) const
			{
				return !(*this == other);
			}

			MemoryBlockMetaData * operator*()
			{
				return m_CurrentMetaData;
			}
		private:
			MemoryBlockMetaData * m_CurrentMetaData;
		};

		MemoryBlockIterator Begin()
		{
			return MemoryBlockIterator(m_FirstMemoryBlock);
		}

		MemoryBlockIterator End()
		{
			return MemoryBlockIterator(nullptr);
		}

	private:
		void InsertMemoryBlock(MemoryBlockMetaData * newBlock, MemoryBlockMetaData * previousBlock, MemoryBlockMetaData * nextBlock);
		void RemoveMemoryBlock(MemoryBlockMetaData * block);

		U32 m_PoolSize;

		U32 m_TotalAllocatedBytes;
		U32 m_AllocationCount;

		U32 m_LargestAllocationBytes;
		U32 m_AverageAllocationBytes;

		U32 m_MinBlockSizeBytes;

		MemoryBlockMetaData * m_FirstMemoryBlock;
		MemoryBlockMetaData * m_FreeSearchBegin;
	};
}