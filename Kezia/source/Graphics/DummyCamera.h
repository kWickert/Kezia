#pragma once

#include "Graphics/Camera.h"

namespace Kezia
{
	class DummyCamera : public Camera
	{
	public:
		DummyCamera();
		virtual ~DummyCamera();

		virtual void PreRender();

		inline void SetViewMatrix(const Matrix4<F32> & viewMatrix);
		inline void SetProjectionMatrix(const Matrix4<F32> & projectionMatrix);
	};

	inline void DummyCamera::SetViewMatrix(const Matrix4<F32> & viewMatrix)
	{
		m_ViewMatrix = viewMatrix;
	}

	inline void DummyCamera::SetProjectionMatrix(const Matrix4<F32> & projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
	}
}