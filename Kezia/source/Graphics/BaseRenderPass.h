#pragma once

#include "IRenderPass.h"

namespace Kezia
{
	class IRenderTarget;

	class BaseRenderPass : public IRenderPass
	{
	public:
		BaseRenderPass(IRenderTarget * target);
		virtual ~BaseRenderPass();

		virtual void PreRender(IRenderPass * previousRenderPass);
		virtual void Render(IRenderer * renderer);
		virtual void PostRender();

		virtual IRenderTarget * GetRenderTarget();

	protected:
		IRenderTarget * m_RenderTarget;
	};
}