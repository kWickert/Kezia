#pragma once

#include "Core/Job.h"
#include "Core/Types.h"

namespace Kezia
{
	class HashBufferJob : public Job
	{
	public:
		HashBufferJob(const char * buffer, const U32 bufferSize, U32 * outHash);
		virtual ~HashBufferJob();

		virtual void Execute();
	private:
		const char * m_Buffer;
		U32 m_BufferSize;
		U32 * m_OutHash;
	};
}