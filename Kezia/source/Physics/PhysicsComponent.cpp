#include "PhysicsComponent.h"

namespace Kezia
{
	PhysicsComponent::PhysicsComponent()
	{

	}

	PhysicsComponent::PhysicsComponent(const PhysicsComponent & other)
	{}

	PhysicsComponent::~PhysicsComponent()
	{}

	AttachableComponent * PhysicsComponent::Clone() const
	{
		return new PhysicsComponent(*this);
	}

	void PhysicsComponent::Update(const F64 deltaTime)
	{

	}

	void PhysicsComponent::EnableKinematicPhysics()
	{

	}

	void PhysicsComponent::EnablePawnPhysics()
	{

	}

	void PhysicsComponent::ApplyImpulse(const Vector3<F32> & impulse)
	{

	}

	void PhysicsComponent::SetLinearVelocity(const Vector3<F32> & velocity)
	{

	}
}