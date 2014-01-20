#pragma once

#include "Graphics/BaseRenderPass.h"

#include <vector>

namespace Kezia
{
	class RenderChain : public BaseRenderPass
	{
	public:
		RenderChain(IRenderTarget * target);
		virtual ~RenderChain();

		virtual void PreRender(IRenderPass * previousRenderPass);
		virtual void Render(IRenderer * renderer);
		virtual void PostRender();

		void AddRenderPass(IRenderPass * renderPass);
	private:
		std::vector<IRenderPass *> m_RenderPasses;
	};
}