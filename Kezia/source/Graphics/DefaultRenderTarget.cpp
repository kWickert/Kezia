#include "DefaultRenderTarget.h"

#include <Gl/glew.h>
#include <gl/GL.h>

namespace Kezia
{
	DefaultRenderTarget::DefaultRenderTarget()
	{}

	DefaultRenderTarget::~DefaultRenderTarget()
	{}

	void DefaultRenderTarget::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DefaultRenderTarget::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DefaultRenderTarget::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}