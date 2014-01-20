#include "StandardRenderPass.h"

#include "Core/Console.h"

#include "Graphics/Camera.h"
#include "Graphics/IRenderTarget.h"
#include "Graphics/OpenGL_Renderer.h"

namespace Kezia
{
	StandardRenderPass::StandardRenderPass(IRenderTarget * target, Camera * camera)
		:	BaseRenderPass(target),
			m_Camera(camera)
	{}

	StandardRenderPass::~StandardRenderPass()
	{}

	void StandardRenderPass::Render(IRenderer * renderer)
	{
		m_Camera->PreRender();
		OpenGLRender(renderer, m_Camera);

		g_Console->Render();

		m_RenderTarget->Unbind();
	}

	void StandardRenderPass::OpenGLRender(IRenderer * renderer, const Camera * camera)
	{
		OpenGL_Renderer * oglRenderer = dynamic_cast<OpenGL_Renderer *>(renderer);

		if(oglRenderer != nullptr)
		{
			RenderableComponent * renderable;

			for(auto it = oglRenderer->RenderableBegin(); it != oglRenderer->RenderableEnd(); ++it)
			{
				renderable = reinterpret_cast<RenderableComponent *>(*it);

				if(renderable->IsAttached() && renderable->IsVisible())
					renderable->Draw(camera);
			}

			oglRenderer->ExecuteRenderOperations();
		}
	}
}