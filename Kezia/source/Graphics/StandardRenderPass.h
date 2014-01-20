#pragma once

#include "Graphics/BaseRenderPass.h"

namespace Kezia
{
	class Camera;

	class StandardRenderPass : public BaseRenderPass
	{
	public:
		StandardRenderPass(IRenderTarget * target, Camera * camera);
		virtual ~StandardRenderPass();

		virtual void Render(IRenderer * renderer);

		static void OpenGLRender(IRenderer * renderer, const Camera * camera);
	private:
		Camera * m_Camera;
	};
}