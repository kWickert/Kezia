#include "OculusCamera.h"

#include "Input/OculusUtility.h"

#include "Math/KeziaTrig.h"

namespace Kezia
{
	OculusCamera::OculusCamera(OculusImplementation * oculusImplementation)
		:	m_OculusImplementation(oculusImplementation),
			m_OffsetOrientation()
	{}

	OculusCamera::~OculusCamera()
	{}

	void OculusCamera::PreRender()
	{
		OVR::HMDInfo & info = m_OculusImplementation->info;

		F32 aspectRatio = info.HResolution * 0.5f / info.VResolution;

		F32 halfScreenDistance = info.VScreenSize / 2;
		F32 yFOV = 2.0f * atan(halfScreenDistance / info.EyeToScreenDistance);

		if(m_OculusImplementation->device == nullptr)
		{
			yFOV = 1.7f;
			aspectRatio = 0.8f;
		}

		Matrix4<F32> baseOrientation = m_Node->GetWorldOrientation();
		baseOrientation.Transpose();

		Matrix4<F32> offsetOrientation = m_OffsetOrientation;
		offsetOrientation.Transpose();

		m_ViewMatrix = offsetOrientation * baseOrientation * Matrix4<F32>::Translate(-m_Node->GetWorldPosition());

		F32 halfInterpupillaryDistance = info.InterpupillaryDistance * 0.5f;

		m_LeftEyeCamera.SetViewMatrix(Matrix4<F32>::Translate(halfInterpupillaryDistance, 0, 0) * m_ViewMatrix);
		m_RightEyeCamera.SetViewMatrix(Matrix4<F32>::Translate(-halfInterpupillaryDistance, 0, 0) * m_ViewMatrix);

		m_ProjectionMatrix = Matrix4<F32>::Perspective(yFOV, aspectRatio, 0.3f, 1000.0f);

		F32 viewCenter = info.HScreenSize * 0.25f;
		F32 eyeProjectionShift = viewCenter - info.LensSeparationDistance * 0.5f;
		F32 projectionCenterOffset = 4.0f * eyeProjectionShift / info.HScreenSize;

		m_LeftEyeCamera.SetProjectionMatrix(Matrix4<F32>::Translate(projectionCenterOffset, 0, 0) * m_ProjectionMatrix);
		m_RightEyeCamera.SetProjectionMatrix(Matrix4<F32>::Translate(-projectionCenterOffset, 0, 0) * m_ProjectionMatrix);
	}

	DummyCamera * OculusCamera::GetLeftEyeCamera()
	{
		return &m_LeftEyeCamera;
	}

	DummyCamera * OculusCamera::GetRightEyeCamera()
	{
		return &m_RightEyeCamera;
	}
}