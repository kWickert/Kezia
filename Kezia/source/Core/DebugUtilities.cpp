#include "DebugUtilities.h"

#include <sstream>

#include <Windows.h>

namespace Kezia
{
	std::string GenerateDebugString(const std::string & file, U32 line, const std::string & errorMessage)
	{
		std::stringstream ss;

		ss << file << '(' << line << "): " << errorMessage << std::endl;

		return ss.str().c_str();
	}

	void WriteDebugString(const std::string & message)
	{
		OutputDebugStringA(message.c_str());
	}

	void ShowModalWindow(const std::string & caption, const std::string & text)
	{
		MessageBoxA(NULL, text.c_str(), caption.c_str(), MB_OK);
	}
}