#pragma once

#include "Physics/BulletPhysicsComponent.h"
#include "Physics/PhysicsComponent.h"

namespace Kezia
{
	class XmlNode;

	class StaticPlanePhysicsComponent : public BulletPhysicsComponent
	{
	public:
		StaticPlanePhysicsComponent(const XmlNode & node);
		StaticPlanePhysicsComponent(const Vector3<F32> & planeNormal, const F32 displacement);
		StaticPlanePhysicsComponent(const StaticPlanePhysicsComponent & other);
		virtual ~StaticPlanePhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void InitializeBulletComponent(BulletComponentImplementation * component);

		inline const Vector3<F32> & GetNormal() const;
		inline const F32 GetDisplacement() const;
	private:
		Vector3<F32> m_PlaneNormal;
		F32 m_Displacement;
	};

	const Vector3<F32> & StaticPlanePhysicsComponent::GetNormal() const
	{
		return m_PlaneNormal;
	}

	const F32 StaticPlanePhysicsComponent::GetDisplacement() const
	{
		return m_Displacement;
	}
}