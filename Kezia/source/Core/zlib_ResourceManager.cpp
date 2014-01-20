#include "zlib_ResourceManager.h"

#include "zlib_FileResource.h"

#include <contrib/minizip/unzip.h>

#include <iostream>

#include "Core/Logger.h"

namespace Kezia
{
	zlib_ResourceManager::zlib_ResourceManager()
	{

	}

	zlib_ResourceManager::~zlib_ResourceManager()
	{

	}

	void zlib_ResourceManager::AddSourceDirectory(const std::string & directoryPath)
	{
		if(unzOpen(directoryPath.c_str()) == NULL)
		{
			LOG("could not open zip directory: " << directoryPath.c_str());
		}
		else
		{
			m_SourceDirectories.push_back(directoryPath);
		}
	}

	const bool zlib_ResourceManager::FileExists(const std::string & fileName) const
	{
		return !GetFilePath(fileName).empty();
	}

	const std::string zlib_ResourceManager::GetFilePath(const std::string & fileName) const
	{
		std::string filePath;

		for(auto dir = m_SourceDirectories.begin(); !filePath.empty() && dir != m_SourceDirectories.end(); ++dir)
		{
			unzFile zipFile = unzOpen(dir->c_str());

			unzGoToFirstFile(zipFile);

			do 
			{
				char tmp[256];
				unzGetCurrentFileInfo64(	zipFile, NULL,
					tmp, sizeof(tmp) - 1,
					NULL, 0, NULL, 0);

				std::string currentFileName(tmp);
				if(currentFileName.find(fileName) != std::string::npos)
				{
					filePath = currentFileName;
					break;
				}
			} while (unzGoToNextFile(zipFile) == UNZ_OK);

			unzClose(zipFile);
		}

		return filePath;
	}

	IResource * zlib_ResourceManager::OpenResourceFile(const std::string & fileName)
	{
		zlib_FileResource * file = nullptr;

		std::string finalPath = GetFilePath(fileName);

		if(finalPath != "")
			file = new zlib_FileResource(finalPath, fileName);

		if(file != nullptr)
			m_OpenResources.push_back(file);

		return file;
	}

	const std::string zlib_ResourceManager::GetWorkingDirectory() const
	{
		return "";
	}
}