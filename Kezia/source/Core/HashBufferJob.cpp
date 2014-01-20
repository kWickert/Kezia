#include "HashBufferJob.h"

#include "Core/SuperFastHash.h"

namespace Kezia
{
	HashBufferJob::HashBufferJob(const char * buffer, const U32 bufferSize, U32 * outHash)
		:	m_Buffer(buffer),
			m_BufferSize(bufferSize),
			m_OutHash(outHash)
	{}

	HashBufferJob::~HashBufferJob()
	{}

	void HashBufferJob::Execute()
	{
		*m_OutHash = SuperFastHash(m_Buffer, m_BufferSize);
	}
}