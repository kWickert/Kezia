#pragma once

#include "Physics/PhysicsComponent.h"

namespace Kezia
{
	struct BulletComponentImplementation;

	class BulletPhysicsComponent : public PhysicsComponent
	{
	public:
		BulletPhysicsComponent();
		BulletPhysicsComponent(const BulletPhysicsComponent & other);
		virtual ~BulletPhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void PostAttach();

		virtual void NotifyTransformUpdate();

		virtual void Update(const F64 deltaTime);

		virtual void EnableKinematicPhysics();
		virtual void EnablePawnPhysics();

		virtual void ApplyImpulse(const Vector3<F32> & impulse);
		virtual void SetLinearVelocity(const Vector3<F32> & velocity);

		virtual void InitializeBulletComponent(BulletComponentImplementation * component);
	
		friend class BulletPhysicsManager;
	private:
		BulletComponentImplementation * m_ComponentImplementation;
	};
}