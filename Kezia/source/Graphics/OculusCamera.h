#pragma once

#include "Graphics/Camera.h"
#include "Graphics/DummyCamera.h"

#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

namespace Kezia
{
	struct OculusImplementation;

	class OculusCamera : public Camera
	{
	public:
		OculusCamera(OculusImplementation * oculusWrapper);
		virtual ~OculusCamera();

		virtual void PreRender();		

		inline void SetOffsetOrientation(const Matrix3<F32> & offsetOrientation);
		
		DummyCamera * GetLeftEyeCamera();
		DummyCamera * GetRightEyeCamera();
	private:
		OculusImplementation * m_OculusImplementation;

		Matrix3<F32> m_OffsetOrientation;

		DummyCamera m_LeftEyeCamera;
		DummyCamera m_RightEyeCamera;
	};

	inline void OculusCamera::SetOffsetOrientation(const Matrix3<F32> & offsetOrientation)
	{
		m_OffsetOrientation = offsetOrientation;
	}
}