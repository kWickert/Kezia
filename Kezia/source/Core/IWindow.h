#pragma once

#include "Core/Types.h"

#include <string>

namespace Kezia
{
	class IWindow
	{
	public:	
		virtual void SetTitle(const std::string & title) = 0;

		virtual const U32 GetWidth() const = 0;
		virtual const U32 GetHeight() const = 0;

		virtual void SetSize(const U32 width, const U32 height) = 0;

		virtual void EnterFullscreenMode() = 0;
		virtual void EnterWindowedMode() = 0;
	};
}