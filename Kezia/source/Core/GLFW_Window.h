#pragma once

#include "Core/Types.h"

#include "IWindow.h"

#include <string>

namespace Kezia
{
	class GLFW_Window : public IWindow
	{
	public:
		GLFW_Window(const std::string & windowTitle, const U32 windowWidth, const U32 windowHeight);
		virtual ~GLFW_Window();

		virtual void SetTitle(const std::string & name);

		virtual const U32 GetWidth() const;
		virtual const U32 GetHeight() const;
		
		virtual void SetSize(const U32 width, const U32 height);
		virtual void SetPosition(const U32 x, const U32 y);

		virtual void EnterFullscreenMode();
		virtual void EnterWindowedMode();
	};
}