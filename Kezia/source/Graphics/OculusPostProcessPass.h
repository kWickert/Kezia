#pragma once

#include "Graphics/PostProcessPass.h"

#include "OVR.h"

namespace Kezia
{
	class OculusCamera;
	class OculusController;
	class StandardRenderPass;
	class TextureRenderTarget;

	class OculusPostProcessPass : public PostProcessPass
	{
	public:
		OculusPostProcessPass(OculusCamera * camera, OculusController * controller);
		virtual ~OculusPostProcessPass();

		virtual void PreRender(IRenderPass * previousRenderPass);
		virtual void Render(IRenderer * renderer);
	private:
		void RenderEyePerspective(const OVR::Util::Render::StereoEyeParams & eyeParams);

		Camera * m_Camera;

		OculusController * m_Controller;
		StandardRenderPass * m_LeftEyeRenderPass;
		StandardRenderPass * m_RightEyeRenderPass;
		TextureRenderTarget * m_CombineRenderTarget;
	};
}