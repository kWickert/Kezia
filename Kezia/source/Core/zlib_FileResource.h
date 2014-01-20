#pragma once

#include "IResource.h"

#include <string>

namespace Kezia
{
	class zlib_FileResource : public IResource
	{
	public:
		zlib_FileResource(const std::string & zipPath, const std::string & fileName);
		virtual ~zlib_FileResource();

		virtual const char * GetByteData();

		virtual const U32 GetDataSize() const;
		virtual void ReadIntoBuffer(char * destinationBuffer) const;
	private:
		static const std::string k_Password;

		const std::string m_ZipPath;
		const std::string m_FileName;
		const char * m_Data;

		U32 m_DataSize;
	};
}