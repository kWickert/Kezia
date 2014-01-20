#pragma once

#include <string>

namespace Kezia
{
	class IResource;

	class IResourceManager
	{
	public:
		virtual void AddSourceDirectory(const std::string & directoryPath) = 0;

		virtual const bool FileExists(const std::string & fileName) const = 0;
		virtual const std::string GetFilePath(const std::string & fileName) const = 0;

		virtual IResource * OpenResourceFile(const std::string & fileName) = 0;

		virtual const std::string GetWorkingDirectory() const = 0;
	};

	extern IResourceManager * g_FileResourceManager;
}