#include "Logger.h"

#include "Core/Time.h"
#include "Core/Windows_ResourceManager.h"

#include <iomanip>

namespace Kezia
{
	Logger::Logger(const std::string & fileName)
		:	m_File(g_FileResourceManager->GetWorkingDirectory() + "\\data\\log\\" + fileName + ".log", std::fstream::out | std::fstream::trunc)
	{
		if(!m_File.is_open())
		{
			MessageBoxA(NULL, "Could not open log file, closing", "Error Creating Log", MB_OK);
			exit(1);
		}

		m_File << std::fixed << std::setprecision(3);
	}

	Logger::~Logger()
	{}

	void Logger::Flush()
	{
		m_File.flush();
	}

	void Logger::PrintTimeStamp()
	{
		m_File << GetLiveTimeSeconds();
	}
}