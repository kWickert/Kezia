#pragma once

#include "Core/Types.h"

namespace Kezia
{
	class IResource
	{
	public:
		virtual ~IResource() 
		{}

		virtual const char * GetByteData() = 0;

		virtual const U32 GetDataSize() const = 0;
		virtual void ReadIntoBuffer(char * destinationBuffer) const = 0;
	};
}