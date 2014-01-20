#include "KeziaResourceManager.h"

#include "Core/Types.h"

#include "Core/Windows_ResourceManager.h"
#include "Core/zlib_ResourceManager.h"

#include "Core/WindowsInclude.h"

namespace Kezia
{
	KeziaResourceManager::KeziaResourceManager(ResourceManagerMode mode)
		:	m_Mode(mode),
			m_PreferredResourceManager(nullptr),
			m_SecondaryResourceManager(nullptr)
	{
		char tmp[255];
		GetCurrentDirectoryA(255, tmp);
		std::string currentDirectory(tmp);

		switch(m_Mode)
		{
		case PreferData:
			m_SecondaryResourceManager = new zlib_ResourceManager();
			m_SecondaryResourceManager->AddSourceDirectory(currentDirectory + "\\Data.zip");
		case DataOnly:
			m_PreferredResourceManager = new Windows_ResourceManager();
			m_PreferredResourceManager->AddSourceDirectory(currentDirectory + "\\Data");
			break;

		case PreferArchive:
			m_SecondaryResourceManager = new Windows_ResourceManager();
			m_SecondaryResourceManager->AddSourceDirectory(currentDirectory + "\\Data");
		case ArchiveOnly:
			m_PreferredResourceManager = new zlib_ResourceManager();
			m_PreferredResourceManager->AddSourceDirectory(currentDirectory + "\\Data.zip");
			break;
		}
	}

	KeziaResourceManager::~KeziaResourceManager()
	{
		delete m_PreferredResourceManager;
		delete m_SecondaryResourceManager;
	}

	void KeziaResourceManager::AddSourceDirectory(const std::string & directoryPath)
	{
		U32 lastDot = directoryPath.find_last_of('.');

		if(lastDot == std::string::npos || directoryPath.substr(lastDot) != ".zip")
		{
			if(m_Mode <= PreferData)
				m_PreferredResourceManager->AddSourceDirectory(directoryPath);
			else if(m_Mode == PreferArchive)
				m_SecondaryResourceManager->AddSourceDirectory(directoryPath);
		}
		else
		{
			if(m_Mode >= ArchiveOnly)
				m_PreferredResourceManager->AddSourceDirectory(directoryPath);
			else if(m_Mode == PreferData)
				m_SecondaryResourceManager->AddSourceDirectory(directoryPath);
		}
	}

	IResource * KeziaResourceManager::OpenResourceFile(const std::string & fileName)
	{
		IResource * resource = nullptr;

		resource = m_PreferredResourceManager->OpenResourceFile(fileName);

		if(resource == nullptr && m_SecondaryResourceManager != nullptr)
		{
			resource = m_SecondaryResourceManager->OpenResourceFile(fileName);
		}

		return resource;
	}

}