#include "Camera.h"

#include "Math/KeziaTrig.h"

namespace Kezia
{
	Camera::Camera()
		:	m_Node(new TransformationNode()),
			m_LookTarget(nullptr),
			m_FieldOfView(DegreesToRadians(45.0f)),
			m_AspectRatio(4.0f / 3.0f),
			m_NearPlane(1.0f),
			m_FarPlane(1000.0f)
	{}

	Camera::~Camera()
	{
		delete m_Node;
	}

	void Camera::PreRender()
	{
		if(m_LookTarget == nullptr)
		{
			Matrix3<F32> orientation = m_Node->GetWorldOrientation();
			orientation.Transpose();

			m_ViewMatrix = Matrix4<F32>(orientation) * Matrix4<F32>::Translate(-m_Node->GetWorldPosition());
		}
		else
		{
			m_ViewMatrix = Matrix4<F32>::LookAt(m_Node->GetWorldPosition(), m_LookTarget->GetWorldPosition());
		}		

		m_ProjectionMatrix = Matrix4<F32>::Perspective(m_FieldOfView, m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	void Camera::Attach(TransformationNode * node)
	{
		node->AddChild(m_Node);
	}

	void Camera::SetLookTarget(TransformationNode * target)
	{
		m_LookTarget = target;
	}
}