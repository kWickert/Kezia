#include "FileResource.h"

#include "Core/Types.h"

#include "Core/WindowsInclude.h"

namespace Kezia
{
	FileResource::FileResource(const std::string & filePath)
		:	m_Path(filePath),
			m_NameBegin(filePath.find_last_of('\\') + 1),
			m_Data(nullptr)
	{
		ReloadData();
	}

	FileResource::~FileResource()
	{
		delete m_Data;
	}

	const char * FileResource::GetByteData()
	{
		return m_Data;
	}

	const U32 FileResource::GetDataSize() const
	{
		return m_DataSize;
	}

	void FileResource::ReadIntoBuffer(char * destinationBuffer) const
	{
		memcpy(destinationBuffer, m_Data, m_DataSize);
	}

	const std::string & FileResource::GetFilePath() const
	{
		return m_Path;
	}

	const std::string FileResource::GetFileName() const
	{
		return m_Path.substr(m_NameBegin);
	}

	void FileResource::ReloadData()
	{
		if(m_Data != nullptr)
		{
			delete m_Data;
			m_Data = nullptr;
		}

		std::ifstream file(m_Path, std::ios::in | std::ios::binary | std::ios::ate);
		file >> std::noskipws;

		if(file.is_open())
		{
			m_DataSize = static_cast<U32>(file.tellg());
			m_Data = new char[m_DataSize + 1];

			file.seekg(0, std::ios::beg);
			file.read(const_cast<char *>(m_Data), m_DataSize);
			file.close();

			const_cast<char &>(m_Data[m_DataSize]) = '\0';
		}
	}
}