#pragma once

#include "Graphics/Texture.h"

#include <string>

namespace Kezia
{
	class IRenderer;
	class IRenderTarget;

	class IRenderPass
	{
	public:
		virtual ~IRenderPass()
		{}

		virtual void PreRender(IRenderPass * previousRenderPass) = 0;
		virtual void Render(IRenderer * renderer) = 0;
		virtual void PostRender() = 0;

		virtual IRenderTarget * GetRenderTarget() = 0;
	};
}