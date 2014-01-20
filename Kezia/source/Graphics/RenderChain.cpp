#include "RenderChain.h"

namespace Kezia
{
	RenderChain::RenderChain(IRenderTarget * target)
		:	BaseRenderPass(target)
	{}

	RenderChain::~RenderChain()
	{}

	void RenderChain::PreRender(IRenderPass * previousRenderPass)
	{
		
	}

	void RenderChain::Render(IRenderer * renderer)
	{
		IRenderPass * previousRenderPass = nullptr;
		for(auto it = m_RenderPasses.begin(); it != m_RenderPasses.end(); ++it)
		{
			(*it)->PreRender(previousRenderPass);
			(*it)->Render(renderer);
			(*it)->PostRender();

			previousRenderPass = *it;
		}
	}

	void RenderChain::PostRender()
	{

	}

	void RenderChain::AddRenderPass(IRenderPass * renderPass)
	{
		m_RenderPasses.push_back(renderPass);
	}
}