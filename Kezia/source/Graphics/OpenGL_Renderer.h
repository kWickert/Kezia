#pragma once

#include "Core/Types.h"
#include "Core/HashedRegistry.h"

#include "Graphics/IRenderer.h"
#include "Graphics/RenderableComponent.h"

#include <vector>

namespace Kezia
{
	class BaseRenderPass;
	class IApplication;
	class Material;
	class RenderOperation;
	class StaticMesh;

	class OpenGL_Renderer : public IRenderer
	{
	private:
		typedef std::vector<RenderableComponent *>::const_iterator ConstRenderableIterator;

	public:
		OpenGL_Renderer();
		virtual ~OpenGL_Renderer();

		virtual Material * GetMaterial(const std::string & name);

		virtual void Clear();
		virtual void Draw();
		virtual void Update(const F64 deltaTime);

		virtual void AddRenderableComponent(RenderableComponent * component);
		virtual void RemoveRenderableComponent(RenderableComponent * component);

		virtual void AddRenderOperation(RenderOperation * operation);
		virtual void RemoveRenderOperation(RenderOperation * operation);
		virtual void ExecuteRenderOperations();

		ConstRenderableIterator RenderableBegin() const;
		ConstRenderableIterator RenderableEnd() const;

		virtual void AddRenderPass(BaseRenderPass * renderPass);

		virtual const Viewport GetCurrentViewport() const;
		virtual void SetViewport(S32 x, S32 y, S32 width, S32 height);
		
		virtual bool IsAlphaBlendingEnabled() const;
		virtual void EnableAlphaBlending();
		virtual void DisableAlphaBlending();

		virtual bool IsDepthTestingEnabled() const;
		virtual void EnableDepthTesting();
		virtual void DisableDepthTesting();

		virtual bool IsTexturingEnabled() const;
		virtual void EnableTexturing();
		virtual void DisableTexturing();

		virtual void EnableWireframeMode();
		virtual void DisableWireframeMode();

		virtual void SetClearColor(const Color & color);
	private:
		friend class IRenderPass;

		HashedRegistry<Material *> m_Materials;

		std::vector<RenderableComponent *> m_RenderableComponents;
		std::vector<RenderOperation *> m_RenderOperations;
		std::vector<BaseRenderPass *> m_RenderPasses;
	};
}