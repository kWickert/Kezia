#include "PoolAllocator.h"

#include "Core/Assert.h"

#include <new>
#include <stdio.h>
#include <stdlib.h>

#include <sstream>

#include "Core/WindowsInclude.h"

namespace Kezia
{
	PoolAllocator::PoolAllocator()
		:	m_PoolSize(0),
			m_TotalAllocatedBytes(0),
			m_AllocationCount(0),
			m_LargestAllocationBytes(0),
			m_MinBlockSizeBytes(50),
			m_FirstMemoryBlock(nullptr)
	{
		const U32 poolSize = 500 * 1024 * 1024;

		Byte * pool = (Byte *)malloc(poolSize);
		
		KeziaAssert(pool != nullptr);
		m_PoolSize = poolSize;

		m_FirstMemoryBlock = new(pool) MemoryBlockMetaData(poolSize - sizeof(MemoryBlockMetaData));
		m_FreeSearchBegin = m_FirstMemoryBlock;
	}

	PoolAllocator::~PoolAllocator()
	{
		ReportMemoryLeaks();
	}

	Byte * PoolAllocator::Allocate(const S32 desiredSizeBytes, const U8 tag, const char * file, int line)
	{
		MemoryBlockMetaData * currentMetaData = m_FreeSearchBegin;

		Byte * memoryBlock = nullptr;
		
		do
		{
			if(currentMetaData->IsFree() && static_cast<U32>(desiredSizeBytes) <= currentMetaData->GetSize())
			{				
				memoryBlock = (Byte *)(currentMetaData + 1);

				U32 spareBytes = currentMetaData->GetSize() - desiredSizeBytes;
				if(spareBytes >= sizeof(MemoryBlockMetaData) + m_MinBlockSizeBytes) //is there enough room to make a new block?
				{
					MemoryBlockMetaData * newBlock = new(memoryBlock + desiredSizeBytes) MemoryBlockMetaData(spareBytes - sizeof(MemoryBlockMetaData), currentMetaData->next, currentMetaData);

					InsertMemoryBlock(newBlock, currentMetaData, currentMetaData->next);
				}

				currentMetaData->sizeBytes = desiredSizeBytes;
				currentMetaData->tag = tag;
				currentMetaData->file = file;
				currentMetaData->line = line;

				m_FreeSearchBegin = currentMetaData;

				m_TotalAllocatedBytes += desiredSizeBytes;
				m_AllocationCount++;

				if(static_cast<U32>(desiredSizeBytes) > m_LargestAllocationBytes)
					m_LargestAllocationBytes = desiredSizeBytes;

				break;
			}

			currentMetaData = currentMetaData->next;

			//loop back to the beginning
			if(currentMetaData == nullptr)
				currentMetaData = m_FirstMemoryBlock;
		}
		while(currentMetaData != m_FreeSearchBegin);

		if(memoryBlock == nullptr)
			throw std::exception("unable to allocate sufficient memory");

		return memoryBlock;
	}

	void PoolAllocator::Deallocate(Byte * pointerData)
	{
		static const U32 MetaDataSize = sizeof(MemoryBlockMetaData);

		//pointerData points to the actual block of memory without the 
		MemoryBlockMetaData * currentMetaData = (MemoryBlockMetaData *)(pointerData - MetaDataSize);
		//mark my memory as free
		currentMetaData->sizeBytes *= -1;

		m_TotalAllocatedBytes -= currentMetaData->GetSize();
		m_AllocationCount--;
		
		MemoryBlockMetaData * previousMetaData = currentMetaData->previous;
		MemoryBlockMetaData * nextMetaData = currentMetaData->next;

		//if the next block is free merge with this one
		while(nextMetaData != nullptr && nextMetaData->IsFree())
		{
			U32 combinedSize = nextMetaData->GetSize() + currentMetaData->GetSize() + MetaDataSize;
			currentMetaData->sizeBytes = -1 * combinedSize;

			RemoveMemoryBlock(nextMetaData);
			nextMetaData = currentMetaData->next;
		}

		//if the previous block is free merge with it
		while(previousMetaData != nullptr && previousMetaData->IsFree())
		{
			U32 combinedSize = previousMetaData->GetSize() + currentMetaData->GetSize() + MetaDataSize;
			previousMetaData->sizeBytes = -1 * combinedSize;

			RemoveMemoryBlock(currentMetaData);
			currentMetaData = previousMetaData;
			previousMetaData = currentMetaData->previous;
		}

		m_FreeSearchBegin = currentMetaData;
	}

	const U32 PoolAllocator::GetPoolSize() const
	{
		return m_PoolSize;
	}

	const U32 PoolAllocator::GetAllocatedBytes() const
	{
		return m_TotalAllocatedBytes;
	}

	const U32 PoolAllocator::GetAllocationCount() const
	{
		return m_AllocationCount;
	}

	const U32 PoolAllocator::GetAverageAllocationSizeBytes() const
	{
		return m_TotalAllocatedBytes / m_AllocationCount;
	}

	void PoolAllocator::InsertMemoryBlock(MemoryBlockMetaData * newBlock, MemoryBlockMetaData * previousBlock, MemoryBlockMetaData * nextBlock)
	{
		if(previousBlock != nullptr)
			previousBlock->next = newBlock;

		if(nextBlock != nullptr)
			nextBlock->previous = newBlock;

		newBlock->next = nextBlock;
		newBlock->previous = previousBlock;
	}

	void PoolAllocator::RemoveMemoryBlock(MemoryBlockMetaData * block)
	{
		MemoryBlockMetaData * nextBlock = block->next;
		MemoryBlockMetaData * previousBlock = block->previous;

		if(nextBlock != nullptr)
			nextBlock->previous = previousBlock;

		if(previousBlock != nullptr)
			previousBlock->next = nextBlock;
	}

	void PoolAllocator::ReportMemoryLeaks()
	{
		const U32 bufferSize = 512;

		char buffer[bufferSize];
		
		for(auto block = Begin(); block != End(); ++block)
		{
			MemoryBlockMetaData * metaData = (*block);

			if(!metaData->IsFree())
			{
				ZeroMemory(buffer, bufferSize);

				sprintf_s(buffer, "%s (%d): found memory leak of size %d bytes\n", 
					metaData->file == nullptr ? "no file information" : metaData->file, 
					metaData->line,
					metaData->GetSize());

				OutputDebugStringA(buffer);

				/*
				if(metaData->file != nullptr)
					ss << metaData->file << '(' << metaData->line << "): ";
				else
				{
					ss << "no file information: ";
				}

				ss << "found memory leak of size " << metaData->GetSize() << " bytes\n"; 
				*/
			}
		}
	}
}