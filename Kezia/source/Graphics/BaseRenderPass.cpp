#include "BaseRenderPass.h"

#include "IRenderTarget.h"

namespace Kezia
{
	BaseRenderPass::BaseRenderPass(IRenderTarget * target)
		:	m_RenderTarget(target)
	{}

	BaseRenderPass::~BaseRenderPass()
	{}

	void BaseRenderPass::PreRender(IRenderPass * previousRenderPass)
	{
		m_RenderTarget->Bind();
		m_RenderTarget->Clear();
	}

	void BaseRenderPass::Render(IRenderer * renderer)
	{}

	void BaseRenderPass::PostRender()
	{
		m_RenderTarget->Unbind();
	}

	IRenderTarget * BaseRenderPass::GetRenderTarget()
	{
		return m_RenderTarget;
	}
}