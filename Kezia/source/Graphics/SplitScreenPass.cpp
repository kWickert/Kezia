#include "SplitScreenPass.h"

#include "Core/IApplication.h"
#include "Core/IWindow.h"

#include "Core/Console.h"

#include "Graphics/Camera.h"
#include "Graphics/IRenderer.h"
#include "Graphics/OpenGL_Renderer.h"
#include "Graphics/StandardRenderPass.h"

namespace Kezia
{
	SplitScreenPass::SplitScreenPass(IRenderTarget * target)
		:	BaseRenderPass(target)
	{}

	SplitScreenPass::~SplitScreenPass()
	{}

	void SplitScreenPass::AddCamera(Camera * camera)
	{
		m_Cameras.push_back(camera);
	}

	void SplitScreenPass::ClearCameras()
	{
		m_Cameras.clear();
	}

	void SplitScreenPass::Render(IRenderer * renderer)
	{
		if(m_Cameras.size() > 2 || m_Cameras.size() < 1)
		{
			throw std::exception("not implemented yet");
		}

		IWindow * window = g_Application->GetMainWindow();

		U32 height = window->GetHeight();
		U32 width = window->GetWidth();
		U32 halfWidth = width / 2;

		m_Cameras[0]->PreRender();
		renderer->SetViewport(0, 0, halfWidth, height);
		StandardRenderPass::OpenGLRender(renderer, m_Cameras[0]);

		m_Cameras[1]->PreRender();
		renderer->SetViewport(halfWidth, 0, halfWidth, height);
		StandardRenderPass::OpenGLRender(renderer, m_Cameras[1]);

		renderer->SetViewport(0, 0, width, height);
		g_Console->Render();
	}
}