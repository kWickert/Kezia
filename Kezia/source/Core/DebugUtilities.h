#pragma once

#include "Core/Types.h"

#include <string>

namespace Kezia
{
	std::string GenerateDebugString(const std::string & file, U32 line, const std::string & errorMessage);
	void WriteDebugString(const std::string & errorMessage);
	void ShowModalWindow(const std::string & caption, const std::string & text);
}