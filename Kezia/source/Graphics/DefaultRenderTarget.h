#pragma once

#include "Graphics/IRenderTarget.h"

namespace Kezia
{
	class DefaultRenderTarget : public IRenderTarget
	{
	public:
		DefaultRenderTarget();
		virtual ~DefaultRenderTarget();

		virtual void Bind();
		virtual void Unbind();

		virtual void Clear();
	};
}