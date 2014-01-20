#include "RenderableComponent.h"

#include "Graphics/IRenderer.h"

namespace Kezia
{
	RenderableComponent::RenderableComponent()
	{}

	RenderableComponent::RenderableComponent(const RenderableComponent & other)
		:	AttachableComponent(other),
			m_IsVisible(other.m_IsVisible),
			m_WireframeEnabled(other.m_WireframeEnabled)
	{}

	RenderableComponent::~RenderableComponent()
	{}

	AttachableComponent * RenderableComponent::Clone() const
	{
		return new RenderableComponent(*this);
	}

	void RenderableComponent::PostAttach()
	{
		g_Renderer->AddRenderableComponent(this);
	}

	void RenderableComponent::Draw(const Camera * camera)
	{}

	void RenderableComponent::Update(const F64 deltaTime)
	{}
}