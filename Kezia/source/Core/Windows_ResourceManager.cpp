#include "Windows_ResourceManager.h"

#include "FileResource.h"

#include <iostream>

#include "Core/WindowsInclude.h"
#include "Core/Logger.h"

namespace Kezia
{
	Windows_ResourceManager::Windows_ResourceManager()
	{}

	Windows_ResourceManager::~Windows_ResourceManager()
	{
		for(auto it = m_OpenResources.begin(); it != m_OpenResources.end(); ++it)
		{
			delete *it;
		}
	}

	const std::string Windows_ResourceManager::GetWorkingDirectory() const
	{
		char tmp[255];
		GetCurrentDirectoryA(255, tmp);
		return std::string(tmp);
	}

	void Windows_ResourceManager::AddSourceDirectory(const std::string & directoryPath)
	{
		DWORD ftyp = GetFileAttributesA(directoryPath.c_str());
		
		if(ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY))
		{
			LOG("the directory " << directoryPath << " does not exist");
		}
		else
		{
			m_SourceDirectories.push_back(directoryPath);
		}
	}

	const bool Windows_ResourceManager::FileExists(const std::string & fileName) const
	{
		std::string finalPath = GetFilePath(fileName);

		return finalPath != "";
	}

	const std::string Windows_ResourceManager::GetFilePath(const std::string & fileName) const
	{
		std::string filePath;

		for(auto dir = m_SourceDirectories.begin(); dir != m_SourceDirectories.end(); ++dir)
		{
			if(SearchDirectory(*dir, fileName, filePath))
				break;
		}

		return filePath;
	}

	IResource * Windows_ResourceManager::OpenResourceFile(const std::string & fileName)
	{
		std::string finalPath = GetFilePath(fileName);

		FileResource * file = nullptr;
		if(finalPath != "")
		{
			file = new FileResource(finalPath);
		}

		m_OpenResources.push_back(file);

		return file;
	}

	void Windows_ResourceManager::LoadDirectoryContents(const std::string & directoryPath, bool recursiveLoad, std::vector<FileResource *> & outLoadedFiles)
	{
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAA ffd;

		std::string search = directoryPath + "\\*";

		hFind = FindFirstFile(search.c_str(), &ffd);

		do 
		{
			std::string currentFileName(ffd.cFileName);

			if(currentFileName != "." && currentFileName != "..")
			{
				if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					LoadDirectoryContents(directoryPath + "\\" + currentFileName, recursiveLoad, outLoadedFiles);
				}
				else
				{
					FileResource * file = new FileResource(directoryPath + "\\" + currentFileName);
					outLoadedFiles.push_back(file);
					m_OpenResources.push_back(file);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	bool Windows_ResourceManager::SearchDirectory(const std::string & directoryName, const std::string & fileName, std::string & outFilePath) const
	{
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAA ffd;

		std::string search = directoryName + "\\*";

		hFind = FindFirstFile(search.c_str(), &ffd);

		do 
		{
			std::string currentFileName(ffd.cFileName);
			
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && currentFileName != "." && currentFileName != "..")
			{
				if(SearchDirectory(directoryName + "\\" + currentFileName, fileName, outFilePath))
					return true;
			}
			else if(currentFileName == fileName)
			{
				outFilePath = directoryName + "\\" + fileName;
				return true;
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		outFilePath = "";
		return false;
	}
}