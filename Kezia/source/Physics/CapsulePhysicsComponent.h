#pragma once

#include "Physics/BulletPhysicsComponent.h"

namespace Kezia
{
	class XmlNode;

	class CapsulePhysicsComponent : public BulletPhysicsComponent
	{
	public:
		CapsulePhysicsComponent(const XmlNode & node);
		CapsulePhysicsComponent(const CapsulePhysicsComponent & other);
		virtual ~CapsulePhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void InitializeBulletComponent(BulletComponentImplementation * component);

		const F32 GetRadius() const;
		const F32 GetHeight() const;

	private:
		F32 m_Radius;
		F32 m_Height;
	};

	inline const F32 CapsulePhysicsComponent::GetRadius() const
	{
		return m_Radius;
	}

	inline const F32 CapsulePhysicsComponent::GetHeight() const
	{
		return m_Height;
	}
}