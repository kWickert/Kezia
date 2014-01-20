#pragma once

#include "IResourceManager.h"

#include <string>
#include <vector>

namespace Kezia
{
	class zlib_ResourceManager : public IResourceManager
	{
	public:
		zlib_ResourceManager();
		virtual ~zlib_ResourceManager();

		virtual void AddSourceDirectory(const std::string & directoryPath);

		virtual const bool FileExists(const std::string & fileName) const;
		virtual const std::string GetFilePath(const std::string & fileName) const;

		virtual IResource * OpenResourceFile(const std::string & fileName);
		
		virtual const std::string GetWorkingDirectory() const;
	private:
		std::vector<std::string> m_SourceDirectories;
		std::vector<IResource *> m_OpenResources;
	};
}