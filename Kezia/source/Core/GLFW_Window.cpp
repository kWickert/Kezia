#include "GLFW_Window.h"

#include <GL/glfw.h>

namespace Kezia
{
	GLFW_Window::GLFW_Window(const std::string & windowTitle, const U32 windowWidth, const U32 windowHeight)
	{
		glfwOpenWindow(windowWidth, windowHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
		SetTitle(windowTitle);
	}

	GLFW_Window::~GLFW_Window()
	{
		glfwCloseWindow();
	}

	void GLFW_Window::SetTitle(const std::string & name)
	{
		glfwSetWindowTitle(name.c_str());
	}

	const U32 GLFW_Window::GetWidth() const
	{
		S32 width;
		glfwGetWindowSize(&width, nullptr);

		return width;
	}

	const U32 GLFW_Window::GetHeight() const
	{
		S32 height;
		glfwGetWindowSize(nullptr, &height);

		return height;
	}

	void GLFW_Window::SetSize(const U32 width, const U32 height)
	{
		glfwSetWindowSize(width, height);
	}

	void GLFW_Window::SetPosition(const U32 x, const U32 y)
	{
		glfwSetWindowPos(x, y);
	}

	void GLFW_Window::EnterFullscreenMode()
	{
		throw std::exception("this function is not implemented yet");
	}

	void GLFW_Window::EnterWindowedMode()
	{
		throw std::exception("this function is not implemented yet");
	}
}