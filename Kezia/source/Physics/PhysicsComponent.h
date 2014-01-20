#pragma once

#include "Core/AttachableComponent.h"

namespace Kezia
{
	class PhysicsComponent : public AttachableComponent
	{
	public:
		PhysicsComponent();
		PhysicsComponent(const PhysicsComponent & other);
		virtual ~PhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void Update(const F64 deltaTime);

		virtual void EnableKinematicPhysics();
		virtual void EnablePawnPhysics();

		virtual void ApplyImpulse(const Vector3<F32> & impulse);
		virtual void SetLinearVelocity(const Vector3<F32> & velocity);

	private:
	};
}