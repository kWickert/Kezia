#pragma once

#include "Graphics/BaseRenderPass.h"

#include <vector>

namespace Kezia
{
	class Camera;
	class IRenderTarget;

	class SplitScreenPass : public BaseRenderPass
	{
	public:
		SplitScreenPass(IRenderTarget * target);
		virtual ~SplitScreenPass();

		void AddCamera(Camera * camera);
		void ClearCameras();

		virtual void Render(IRenderer * renderer);

	private:
		std::vector<Camera *> m_Cameras;
	};
}