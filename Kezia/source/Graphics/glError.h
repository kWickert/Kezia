#pragma once

#include "Core/Logger.h"

#include "Graphics/glInclude.h"

#include <string>

namespace Kezia
{
	inline bool CheckGLErrors(const std::string & tag = "")
	{
		GLenum error = glGetError();

		switch(error)
		{
		case GL_NO_ERROR:
				LOG(tag << ": GL_NO_ERROR");
				break;
		case GL_INVALID_ENUM:
				LOG(tag << ": GL_INVALID_ENUM");
				break;
		case GL_INVALID_VALUE:
				LOG(tag << ": GL_INVALID_VALUE");
				break;
		case GL_INVALID_OPERATION:
				LOG(tag << ": GL_INVALID_OPERATION");
				break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
				LOG(tag << ": GL_INVALID_FRAMEBUFFER_OPERATION");
				break;
		case GL_OUT_OF_MEMORY:
				LOG(tag << ": GL_OUT_OF_MEMORY");
				break;
		}

		return error != GL_NO_ERROR;
	}

	inline void ReportAllGLErrors(const std::string & tag = "")
	{
		while(CheckGLErrors(tag))
		{
			;
		}
	}
}
