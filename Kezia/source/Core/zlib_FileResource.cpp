#include "zlib_FileResource.h"

#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/Types.h"

#include <contrib/minizip/unzip.h>

#include <iostream>

namespace Kezia
{
	const std::string zlib_FileResource::k_Password("phone");

	zlib_FileResource::zlib_FileResource(const std::string & zipPath, const std::string & fileName)
		:	m_ZipPath(zipPath),
			m_FileName(fileName)
	{
		unzFile zipFile = unzOpen(zipPath.c_str());

		KeziaAssert(unzLocateFile(zipFile, fileName.c_str(), 0) == UNZ_OK);

		unz_file_info fileInfo;
		unzGetCurrentFileInfo(zipFile, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0);

		m_Data = new char[fileInfo.uncompressed_size + 1];

		if(unzOpenCurrentFilePassword(zipFile, k_Password.c_str()) != UNZ_OK)
		{
			LOG("wrong password");
		}
		
		m_DataSize = unzReadCurrentFile(zipFile, const_cast<char *>(m_Data), fileInfo.uncompressed_size);
		const_cast<char &>(m_Data[m_DataSize]) = '\0';
		
		unzCloseCurrentFile(zipFile);
		
		unzClose(zipFile);
	}

	zlib_FileResource::~zlib_FileResource()
	{}

	const char * zlib_FileResource::GetByteData()
	{
		return m_Data;
	}

	const U32 zlib_FileResource::GetDataSize() const
	{
		return m_DataSize;
	}

	void zlib_FileResource::ReadIntoBuffer(char * destinationBuffer) const
	{
		memcpy(destinationBuffer, m_Data, m_DataSize);
	}
}