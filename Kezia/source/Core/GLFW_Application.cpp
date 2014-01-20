#include "GLFW_Application.h"
#include "GLFW_Window.h"

#include "Core/Clock.h"
#include "Core/Logger.h"
#include "Core/Time.h"

#include "Math/MathUtility.h"

#include <GL/glfw.h>

namespace Kezia
{
	Clock * g_MasterClock;

	GLFW_Application::GLFW_Application()
		:	m_CurrentWindow(nullptr),
			m_TargetFrameDuration(1.0 / 60.0),
			m_Quitting(false)
	{
		if(glfwInit() != GL_TRUE)
			throw std::exception("Failed to initialize GLFW application");

		g_MasterClock = new Clock();
	}

	GLFW_Application::~GLFW_Application()
	{
		glfwTerminate();

		for(auto it = m_Windows.Begin(); it != m_Windows.End(); ++it)
		{
			delete it->second;
		}
	}

	IWindow * GLFW_Application::CreateWindow(const std::string & windowTitle, const U32 windowWidth, const U32 windowHeight)
	{
		GLFW_Window * window = new GLFW_Window(windowTitle, windowWidth, windowHeight);
		m_Windows.Insert(windowTitle, window);

		return window;
	}

	IWindow * GLFW_Application::GetWindow(const std::string & windowTitle)
	{
		auto findResult = m_Windows.Find(windowTitle);

		if(findResult == m_Windows.End())
			return nullptr;
		else
			return findResult->second;
	}

	IWindow * GLFW_Application::GetMainWindow()
	{
		auto first = m_Windows.Begin();

		if(first != m_Windows.End())
			return first->second;
		else
			return nullptr;
	}

	void GLFW_Application::SwapBuffers()
	{
		glfwSwapBuffers();
	}

	void GLFW_Application::Run()
	{
		F64 previousFrameTimestamp = GetLiveTimeSeconds();

		for(; !m_Quitting;)
		{
			F64 currentFrameTimestamp = 0;
			F64 frameDuration = 0;

			do 
			{
				currentFrameTimestamp = GetLiveTimeSeconds();
				frameDuration = currentFrameTimestamp - previousFrameTimestamp;
			} while (frameDuration < m_TargetFrameDuration);

			m_FrameRateHistory.push_back(1.0 / frameDuration);

			if(m_FrameRateHistory.size() > k_TargetFrameHistorySize)
				m_FrameRateHistory.pop_front();

			U32 i = 0;
			F64 total = 0;
			for(auto it = m_FrameRateHistory.begin(); it != m_FrameRateHistory.end(); ++it, ++i)
			{
				total += *it;
			}

			m_CurrentFrameRate = total / i;

			Update(frameDuration);
			g_MasterClock->AdvanceClock(frameDuration);

			previousFrameTimestamp = currentFrameTimestamp;
		}

		OnShutdown();
	}

	void GLFW_Application::Quit()
	{
		m_Quitting = true;
	}

	void GLFW_Application::OnShutdown()
	{
		LOG("shutting down");
	}
}