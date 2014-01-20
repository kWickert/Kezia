#pragma once

#include "Core/Types.h"

#include "Graphics/IRenderTarget.h"
#include "Graphics/Texture.h"

namespace Kezia
{
	class TextureRenderTarget : public IRenderTarget
	{
	public:
		TextureRenderTarget(const U32 width, const U32 height);
		virtual ~TextureRenderTarget();

		virtual void Bind();
		virtual void Unbind();

		Texture GetColorTexture();
		Texture GetDepthTexture();

		virtual void Clear();
	private:
		static void DepthTextureSetup(void * texels, U32 width, U32 height, bool isTransparent);

		U32 m_Width;
		U32 m_Height;

		U32 m_FboId;

		Texture m_ColorTexture;
		Texture m_DepthTexture;
	};
}