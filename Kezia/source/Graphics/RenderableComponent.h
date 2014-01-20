#pragma once

#include "Core/AttachableComponent.h"

namespace Kezia
{
	class Camera;

	class RenderableComponent : public AttachableComponent
	{
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent & other);
		virtual ~RenderableComponent();

		virtual AttachableComponent * Clone() const;

		virtual void PostAttach();

		virtual void Draw(const Camera * camera);
		virtual void Update(const F64 deltaTime);

		const bool IsWireframeEnabled() const;
		void EnableWireframe();
		void DisableWireframe();

		const bool IsVisible() const;
		void SetVisibility(const bool shouldBeVisible);

	private:
		bool m_IsVisible;
		bool m_WireframeEnabled;
	};

	inline const bool RenderableComponent::IsWireframeEnabled() const
	{
		return m_WireframeEnabled;
	}

	inline void RenderableComponent::EnableWireframe()
	{
		m_WireframeEnabled = true;
	}

	inline void RenderableComponent::DisableWireframe()
	{
		m_WireframeEnabled = false;
	}

	inline const bool RenderableComponent::IsVisible() const
	{
		return m_IsVisible;
	}

	inline void RenderableComponent::SetVisibility(const bool shouldBeVisible)
	{
		m_IsVisible = shouldBeVisible;
	}
}