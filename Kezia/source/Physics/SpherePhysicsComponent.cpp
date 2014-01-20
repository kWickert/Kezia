#include "SpherePhysicsComponent.h"

#include "Core/XmlNode.h"

#include "Physics/BulletUtility.h"

namespace Kezia
{
	SpherePhysicsComponent::SpherePhysicsComponent(const XmlNode & node)
	{
		node.ValidateAttributes("radius", "");

		m_Radius = GetAttribute(node, "radius", 0.0f);
	}

	SpherePhysicsComponent::SpherePhysicsComponent(const F32 radius)
		:	m_Radius(radius)
	{}

	SpherePhysicsComponent::SpherePhysicsComponent(const SpherePhysicsComponent & other)
		:	BulletPhysicsComponent(other),
			m_Radius(other.m_Radius)
	{}

	SpherePhysicsComponent::~SpherePhysicsComponent()
	{}

	AttachableComponent * SpherePhysicsComponent::Clone() const
	{
		return new SpherePhysicsComponent(*this);
	}

	void SpherePhysicsComponent::InitializeBulletComponent(BulletComponentImplementation * component)
	{
		component->shape = new btSphereShape(m_Radius);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), ToBulletVector(GetWorldPosition())));

		F32 mass = 1.0f;

		btVector3 inertia;
		component->shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo ci(mass, motionState, component->shape, inertia);

		component->rigidBody = new btRigidBody(ci);

		BulletPhysicsComponent::InitializeBulletComponent(component);
	}
}