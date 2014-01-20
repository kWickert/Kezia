#include "TextureRenderTarget.h"

#include "Core/Logger.h"

#include <Gl/glew.h>
#include <gl/GL.h>

namespace Kezia
{
	TextureRenderTarget::TextureRenderTarget(const U32 width, const U32 height)
		:	m_Width(width),
			m_Height(height),
			m_ColorTexture(nullptr, width, height, false),
			m_DepthTexture(nullptr, width, height, false, DepthTextureSetup)
	{
		glGenFramebuffers(1, &m_FboId);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture.GetId(), 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture.GetId(), 0);

		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG("framebuffer is incomplete");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	TextureRenderTarget::~TextureRenderTarget()
	{}

	void TextureRenderTarget::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
	}

	void TextureRenderTarget::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void TextureRenderTarget::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void TextureRenderTarget::DepthTextureSetup(void * texels, U32 width, U32 height, bool isTransparent)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, texels);
	}

	Texture TextureRenderTarget::GetColorTexture()
	{
		return m_ColorTexture;
	}

	Texture TextureRenderTarget::GetDepthTexture()
	{
		return m_DepthTexture;
	}
}