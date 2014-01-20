#pragma once

#include "Core/TransformationNode.h"

#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

namespace Kezia
{
	class Camera
	{
	public:
		Camera();
		virtual ~Camera();

		virtual void PreRender();

		void Attach(TransformationNode * node);
		void SetLookTarget(TransformationNode * target);

		inline const Vector3<F32> & GetPosition() const;
		inline void SetPosition(const Vector3<F32> & position);

		inline const Matrix3<F32> & GetOrientation() const;
		inline void SetOrientation(const Quaternion<F32> & orientation);
		inline void SetOrientation(const Matrix3<F32> & orientation);
		inline void Rotate(const F32 yawRadians = 0, const F32 pitchRadians = 0, const F32 rollRadians = 0);

		inline const Matrix4<F32> GetViewMatrix() const;
		inline const Matrix4<F32> GetProjectionMatrix() const;

		inline const F32 GetFieldOfView() const;
		inline void SetFieldOfView(const F32 fov);

		inline const F32 GetAspectRatio() const;
		inline void SetAspectRatio(const F32 ratio);

		inline const F32 GetNearPlane() const;
		inline void SetNearPlane(const F32 nearPlane);

		inline const F32 GetFarPlane() const;
		inline void SetFarPlane(const F32 farPlane);

	protected:
		TransformationNode * m_Node;
		TransformationNode * m_LookTarget;

		Matrix4<F32> m_ViewMatrix;
		Matrix4<F32> m_ProjectionMatrix;

		F32 m_FieldOfView;
		F32 m_AspectRatio;
		F32 m_NearPlane;
		F32 m_FarPlane;
	};

	inline const Vector3<F32> & Camera::GetPosition() const
	{
		return m_Node->GetPosition();
	}

	inline void Camera::SetPosition(const Vector3<F32> & position)
	{
		m_Node->SetPosition(position);
	}

	inline const Matrix3<F32> & Camera::GetOrientation() const
	{
		return m_Node->GetOrientation();
	}

	inline void Camera::SetOrientation(const Quaternion<F32> & orientation)
	{
		m_Node->SetOrientation(orientation);
	}

	inline void Camera::SetOrientation(const Matrix3<F32> & orientation)
	{
		m_Node->SetOrientation(orientation);
	}

	inline void Camera::Rotate(const F32 yawRadians, const F32 pitchRadians, const F32 rollRadians)
	{
		Matrix3<F32> orientation = m_Node->GetOrientation();
		orientation = Matrix3<F32>::RotateY(yawRadians) * orientation;

		Vector3<F32> localX = orientation * Vector3<F32>(1, 0, 0);

		m_Node->SetOrientation(Matrix3<F32>::Rotate(pitchRadians, localX) * orientation);
	}

	inline const Matrix4<F32> Camera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	inline const Matrix4<F32> Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	inline const F32 Camera::GetFieldOfView() const
	{
		return m_FieldOfView;
	}

	inline void Camera::SetFieldOfView(const F32 fov)
	{
		m_FieldOfView = fov;
	}

	inline const F32 Camera::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	inline void Camera::SetAspectRatio(const F32 ratio)
	{
		m_AspectRatio = ratio;
	}

	inline const F32 Camera::GetNearPlane() const
	{
		return m_NearPlane;
	}

	inline void Camera::SetNearPlane(const F32 nearPlane)
	{
		m_NearPlane = nearPlane;
	}

	inline const F32 Camera::GetFarPlane() const
	{
		return m_FarPlane;
	}

	inline void Camera::SetFarPlane(const F32 farPlane)
	{
		m_FarPlane = farPlane;
	}
}