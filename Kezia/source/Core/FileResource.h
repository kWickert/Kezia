#pragma once

#include "IResource.h"

#include "Core/Types.h"

#include <fstream>
#include <string>

namespace Kezia
{
	class FileResource : public IResource
	{
	public:
		FileResource(const std::string & filePath);
		virtual ~FileResource();

		virtual const char * GetByteData();

		virtual const U32 GetDataSize() const;
		virtual void ReadIntoBuffer(char * destinationBuffer) const;

		const std::string & GetFilePath() const;
		const std::string GetFileName() const;
	private:
		void ReloadData();

		const std::string m_Path;
		const U32 m_NameBegin;

		const char * m_Data;

		U32 m_DataSize;
	};
}