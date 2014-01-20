#pragma once

#include "Graphics/BaseRenderPass.h"

namespace Kezia
{
	class Camera;
	class Material;

	class PostProcessPass : public BaseRenderPass
	{
	public:
		PostProcessPass(Material * postProcessEffect);
		virtual ~PostProcessPass();

		virtual void PreRender(IRenderPass * previousRenderPass);
		virtual void Render(IRenderer * renderer);

	protected:
		Material * m_PostProcessMaterial;
	};
}