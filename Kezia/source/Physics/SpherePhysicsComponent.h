#pragma once

#include "Physics/BulletPhysicsComponent.h"

namespace Kezia
{
	class XmlNode;

	class SpherePhysicsComponent : public BulletPhysicsComponent
	{
	public:
		SpherePhysicsComponent(const XmlNode & node);
		SpherePhysicsComponent(const F32 radius);
		SpherePhysicsComponent(const SpherePhysicsComponent & other);
		virtual ~SpherePhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void InitializeBulletComponent(BulletComponentImplementation * component);

		const F32 GetRadius() const;
	private:
		F32 m_Radius;
	};

	inline const F32 SpherePhysicsComponent::GetRadius() const
	{
		return m_Radius;
	}
}