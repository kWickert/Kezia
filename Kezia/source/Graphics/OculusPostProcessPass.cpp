#include "OculusPostProcessPass.h"

#include "Core/IApplication.h"
#include "Core/IWindow.h"

#include "Graphics/DefaultRenderTarget.h"
#include "Graphics/DummyCamera.h"
#include "Graphics/GraphicsCommon.h"
#include "Graphics/IRenderer.h"
#include "Graphics/Material.h"
#include "Graphics/OculusCamera.h"
#include "Graphics/TextureRenderTarget.h"
#include "Graphics/StandardRenderPass.h"

#include "Input/OculusController.h"

namespace Kezia
{
	OculusPostProcessPass::OculusPostProcessPass(OculusCamera * camera, OculusController * controller)
		:	PostProcessPass(reinterpret_cast<Material *>(g_Renderer->GetMaterial("Oculus"))),
			m_Camera(camera),
			m_Controller(controller)
	{
		IWindow * mainWindow = g_Application->GetMainWindow();
		S32 windowWidth = mainWindow->GetWidth();
		S32 windowHeight = mainWindow->GetHeight();
		
		m_LeftEyeRenderPass = new StandardRenderPass(new TextureRenderTarget(windowWidth / 2, windowHeight), camera->GetLeftEyeCamera());
		m_RightEyeRenderPass = new StandardRenderPass(new TextureRenderTarget(windowWidth / 2, windowHeight), camera->GetRightEyeCamera());

		m_CombineRenderTarget = new TextureRenderTarget(windowWidth, windowHeight);

		PositionBufferObject * pBuffer = dynamic_cast<PositionBufferObject *>(m_PostProcessMaterial->GetAttribute("a_Position"));

		if(pBuffer == nullptr)
		{
			m_PostProcessMaterial->GetAttribute("a_Position")->SetBufferObject(new PositionBufferObject(defaultQuadPositions, 4));
		}
		else
		{
			pBuffer->UpdateData(defaultQuadPositions, 4);
		}

		TextureCoordinateBufferObject * tcBuffer = dynamic_cast<TextureCoordinateBufferObject *>(m_PostProcessMaterial->GetAttribute("a_TextureCoordinates"));

		if(tcBuffer == nullptr)
		{
			m_PostProcessMaterial->GetAttribute("a_TextureCoordinates")->SetBufferObject(new TextureCoordinateBufferObject(defaultQuadTexCoords, 4));
		}
		else
		{
			tcBuffer->UpdateData(defaultQuadTexCoords, 4);
		}	
	}

	OculusPostProcessPass::~OculusPostProcessPass()
	{}

	void OculusPostProcessPass::PreRender(IRenderPass * previousRenderPass)
	{
		/*
		TextureRenderTarget * texture = dynamic_cast<TextureRenderTarget *>(previousRenderPass->GetRenderTarget());

		if(texture)
		{
			m_PostProcessMaterial->SetUniformValue("u_SceneRenderTexture", texture->GetColorTexture());
		}
		*/
		m_Camera->PreRender();

		BaseRenderPass::PreRender(previousRenderPass);
	}

	void OculusPostProcessPass::Render(IRenderer * renderer)
	{
		TextureRenderTarget * fullRenderTarget = nullptr;

		IWindow * mainWindow = g_Application->GetMainWindow();

		S32 windowWidth = mainWindow->GetWidth();
		S32 windowHeight = mainWindow->GetHeight();
		S32 halfWidth = windowWidth / 2;

		g_Renderer->SetViewport(0, 0, windowWidth, windowHeight);

		//render the left eye to texture
		m_LeftEyeRenderPass->PreRender(nullptr);
		m_LeftEyeRenderPass->Render(g_Renderer);

		fullRenderTarget = dynamic_cast<TextureRenderTarget *>(m_LeftEyeRenderPass->GetRenderTarget()); 
		Texture leftRender = fullRenderTarget->GetColorTexture();		

		//render the right eye to texture
		m_RightEyeRenderPass->PreRender(nullptr);
		m_RightEyeRenderPass->Render(g_Renderer);

		fullRenderTarget = dynamic_cast<TextureRenderTarget *>(m_RightEyeRenderPass->GetRenderTarget()); 
		Texture rightRender = fullRenderTarget->GetColorTexture();

		//combine the two renders
		m_CombineRenderTarget->Bind();
		m_CombineRenderTarget->Clear();

		g_Renderer->SetViewport(0, 0, halfWidth, windowHeight);
		DrawTexturedQuad(leftRender, Color::White);

		g_Renderer->SetViewport(halfWidth, 0, halfWidth, windowHeight);
		DrawTexturedQuad(rightRender, Color::White);

		//combine the two renders with distortion correction
		OVR::Util::Render::StereoConfig config;

		config.SetFullViewport(OVR::Util::Render::Viewport(0, 0, windowWidth, windowHeight));
		config.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
		config.SetHMDInfo(m_Controller->m_Info);
		config.SetDistortionFitPointVP(-1.0f, 0.0f);

		PreRender(nullptr);

		m_PostProcessMaterial->SetUniformValue("u_SceneRenderTexture", m_CombineRenderTarget->GetColorTexture());

		g_Renderer->SetViewport(0, 0, halfWidth, windowHeight);
		RenderEyePerspective(config.GetEyeRenderParams(OVR::Util::Render::StereoEye_Left));

		g_Renderer->SetViewport(halfWidth, 0, halfWidth, windowHeight);
		RenderEyePerspective(config.GetEyeRenderParams(OVR::Util::Render::StereoEye_Right));
	}

	void OculusPostProcessPass::RenderEyePerspective(const OVR::Util::Render::StereoEyeParams & eyeParams)
	{
		IWindow * mainWindow = g_Application->GetMainWindow();
		S32 windowWidth = mainWindow->GetWidth();
		S32 windowHeight = mainWindow->GetHeight();

		const Viewport & vp = g_Renderer->GetCurrentViewport();

		F32 w = F32(vp.width) / F32(windowWidth);
		F32 h = F32(vp.height) / F32(windowHeight);
		F32 x = F32(vp.x) / F32(windowWidth);
		F32 y = F32(vp.y) / F32(windowHeight);
		F32 aspectRatio = F32(vp.width) / F32(vp.height);	

		const OVR::Util::Render::DistortionConfig * distortion = eyeParams.pDistortion;

		F32 xCenterOffset = (eyeParams.Eye == OVR::Util::Render::StereoEye_Left ? 1 : -1) * distortion->XCenterOffset;
		Vector2<F32> lensCenter(x + (w + xCenterOffset * 0.5f) * 0.5f, y + h * 0.5f);
		Vector2<F32> screenCenter(x + w * 0.5f, y + h * 0.5f);

		m_PostProcessMaterial->SetUniformValue("u_LensCenter", lensCenter);
		m_PostProcessMaterial->SetUniformValue("u_ScreenCenter", screenCenter);

		F32 scaleFactor = 1.0f / distortion->Scale;

		Vector2<F32> scaleOut((w / 2) * scaleFactor, (h / 2) * scaleFactor * aspectRatio);
		Vector2<F32> scaleIn((2 / w), (2 / h) / aspectRatio);

		m_PostProcessMaterial->SetUniformValue("u_ScaleOut", scaleOut);
		m_PostProcessMaterial->SetUniformValue("u_ScaleIn", scaleIn);

		m_PostProcessMaterial->SetUniformValue("u_HmdWarpParam", Vector4<F32>(distortion->K));

		Matrix4<F32> texM(	w, 0, 0, 0,
							0, h, 0, 0,
							0, 0, 0, 0,
							x, y, 0, 1);

		m_PostProcessMaterial->SetUniformValue("u_TextureM", texM);

		Matrix4<F32> view(	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 0, 0,
							0, 0, 0, 1);

		m_PostProcessMaterial->SetUniformValue("u_View", view);

		m_PostProcessMaterial->Bind();	

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}