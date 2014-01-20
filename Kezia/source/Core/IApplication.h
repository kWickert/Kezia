#pragma once

#include "Core/Types.h"

#include <string>

#undef CreateWindowA

namespace Kezia
{
	class HashedString;
	class IWindow;

	class IApplication
	{
	public:
		virtual IWindow * CreateWindow(const std::string & windowTitle, const U32 windowWidth, const U32 windowHeight) = 0;
		virtual IWindow * GetWindow(const std::string & windowTitle) = 0;

		virtual IWindow * GetMainWindow() = 0;

		virtual void Update(const F64 deltaTime) = 0;
		virtual void SwapBuffers() = 0;

		virtual void Run() = 0;
	};

	extern IApplication * g_Application;
}