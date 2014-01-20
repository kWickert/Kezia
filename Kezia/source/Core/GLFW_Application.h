#pragma once

#include "IApplication.h"

#include "Core/Clock.h"
#include "Core/HashedRegistry.h"

#include <list>
#include <map>
#include <string>

namespace Kezia
{
	class GLFW_Application : public IApplication
	{
	public:
		GLFW_Application();
		virtual ~GLFW_Application();

		virtual IWindow * CreateWindow(const std::string & windowTitle, const U32 windowWidth, const U32 windowHeight);
		virtual IWindow * GetWindow(const std::string & windowTitle);

		virtual IWindow * GetMainWindow();

		virtual void Update(const F64 deltaTime) = 0;
		virtual void SwapBuffers();

		virtual void Run();
		void Quit();

		virtual void OnShutdown();
	protected:
		F64 m_CurrentFrameRate;
		std::list<F64> m_FrameRateHistory;
	private:
		bool m_Quitting;

		static const U32 k_TargetFrameHistorySize = 30;

		IWindow * m_CurrentWindow;
		HashedRegistry<IWindow *> m_Windows;

		F64 m_TargetFrameDuration;
		
	};
}