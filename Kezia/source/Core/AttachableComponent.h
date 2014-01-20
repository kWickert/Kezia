#pragma once

#include "Core/TransformationNode.h"

#include "Math/Matrix4.h"

namespace Kezia
{
	class TransformationNode;

	class AttachableComponent
	{
	public:
		AttachableComponent();
		AttachableComponent(const AttachableComponent & other);
		virtual ~AttachableComponent();

		virtual AttachableComponent * Clone() const;

		void Attach(TransformationNode * node);
		virtual void PostAttach();

		void Detach();

		inline bool IsAttached() const;

		inline const Matrix4<F32> GetWorldTransform() const;
		inline const Vector3<F32> GetWorldPosition() const;

		friend class TransformationNode;
	protected:
		virtual void NotifyTransformUpdate();

		TransformationNode * m_Node;
	};

	bool AttachableComponent::IsAttached() const
	{
		return m_Node != nullptr;
	}

	const Matrix4<F32> AttachableComponent::GetWorldTransform() const
	{
		Matrix4<F32> worldTransform;

		if(IsAttached())
		{
			worldTransform = m_Node->GetWorldTransform();
		}

		return worldTransform;
	}

	const Vector3<F32> AttachableComponent::GetWorldPosition() const
	{
		Vector3<F32> worldPosition;
	
		if(IsAttached())
		{
			worldPosition = m_Node->GetWorldPosition();
		}
		
		return worldPosition;
	}
}