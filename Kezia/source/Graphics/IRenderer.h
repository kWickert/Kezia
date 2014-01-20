#pragma once

#include "Graphics/Color.h"
#include "Core/Types.h"

namespace Kezia
{
	class Camera;
	class Material;
	class RenderableComponent;
	class BaseRenderPass;
	class RenderOperation;

	struct Viewport
	{
		S32 x;
		S32 y;
		S32 width;
		S32 height;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer()
		{}

		virtual void Clear() = 0;
		virtual void Draw() = 0;
		virtual void Update(const F64 deltaTime) = 0;

		virtual Material * GetMaterial(const std::string & name) = 0;

		virtual void AddRenderableComponent(RenderableComponent * component) = 0;
		virtual void RemoveRenderableComponent(RenderableComponent * component) = 0;

		virtual void AddRenderOperation(RenderOperation * operation) = 0;
		virtual void RemoveRenderOperation(RenderOperation * operation) = 0;
		virtual void ExecuteRenderOperations() = 0;

		virtual void AddRenderPass(BaseRenderPass * renderPass) = 0;

		virtual const Viewport GetCurrentViewport() const = 0;
		virtual void SetViewport(S32 x, S32 y, S32 width, S32 height) = 0;
		
		virtual bool IsAlphaBlendingEnabled() const = 0;
		virtual void EnableAlphaBlending() = 0;
		virtual void DisableAlphaBlending() = 0;

		virtual bool IsDepthTestingEnabled() const = 0;
		virtual void EnableDepthTesting() = 0;
		virtual void DisableDepthTesting() = 0;

		virtual bool IsTexturingEnabled() const = 0;
		virtual void EnableTexturing() = 0;
		virtual void DisableTexturing() = 0;

		virtual void EnableWireframeMode() = 0;
		virtual void DisableWireframeMode() = 0;

		virtual void SetClearColor(const Color & color) = 0;
	};

	extern IRenderer * g_Renderer;
}