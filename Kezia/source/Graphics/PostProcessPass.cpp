#include "PostProcessPass.h"

#include "Graphics/DummyCamera.h"
#include "Graphics/DefaultRenderTarget.h"
#include "Graphics/GraphicsCommon.h"
#include "Graphics/Material.h"
#include "Graphics/TextureRenderTarget.h"
#include "Graphics/StandardRenderPass.h"

namespace Kezia
{
	PostProcessPass::PostProcessPass(Material * fullScreenEffect)
		:	BaseRenderPass(new DefaultRenderTarget()),
			m_PostProcessMaterial(fullScreenEffect)
	{}

	PostProcessPass::~PostProcessPass()
	{}

	void PostProcessPass::PreRender(IRenderPass * previousRenderPass)
	{
		TextureRenderTarget * texture = dynamic_cast<TextureRenderTarget *>(previousRenderPass->GetRenderTarget());
		
		if(texture)
		{
			m_PostProcessMaterial->SetUniformValue("u_FullScreenTexture", texture->GetColorTexture());
		}

		BaseRenderPass::PreRender(previousRenderPass);
	}

	void PostProcessPass::Render(IRenderer * renderer)
	{
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

		m_PostProcessMaterial->Bind();	

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}