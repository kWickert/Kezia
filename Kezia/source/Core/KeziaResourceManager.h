#pragma once

#include "IResourceManager.h"

namespace Kezia
{
	class KeziaResourceManager : public IResourceManager
	{
	public:
		enum ResourceManagerMode
		{
			DataOnly,
			PreferData,
			ArchiveOnly,
			PreferArchive
		};

		KeziaResourceManager(ResourceManagerMode mode);
		~KeziaResourceManager();

		static const std::string GetDataDirectory();

		virtual void AddSourceDirectory(const std::string & directoryPath);

		virtual IResource * OpenResourceFile(const std::string & fileName);

	private:
		ResourceManagerMode m_Mode;

		IResourceManager * m_PreferredResourceManager;
		IResourceManager * m_SecondaryResourceManager;
	};
}