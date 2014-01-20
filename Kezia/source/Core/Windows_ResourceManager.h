#pragma once

#include "Core/IResourceManager.h"

#include <string>
#include <vector>

namespace Kezia
{
	class FileResource;

	class Windows_ResourceManager : public IResourceManager
	{
	public:
		Windows_ResourceManager();
		virtual ~Windows_ResourceManager();

		const std::string GetWorkingDirectory() const;

		virtual void AddSourceDirectory(const std::string & directoryPath);

		virtual const bool FileExists(const std::string & fileName) const;
		virtual const std::string GetFilePath(const std::string & fileName) const;

		virtual IResource * OpenResourceFile(const std::string & fileName);

		void LoadDirectoryContents(const std::string & directoryPath, bool recursiveLoad, std::vector<FileResource *> & outLoadedFiles);
	private:
		bool SearchDirectory(const std::string & directoryName, const std::string & fileName, std::string & outFilePath) const;

		std::vector<std::string> m_SourceDirectories;
		std::vector<IResource *> m_OpenResources;
	};
}