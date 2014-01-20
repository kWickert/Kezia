#include "BoxPhysicsComponent.h"

#include "Core/XmlNode.h"

#include "Physics/BulletUtility.h"

namespace Kezia
{
	BoxPhysicsComponent::BoxPhysicsComponent(const XmlNode & node)
	{
		node.ValidateAttributes("halfExtents","");

		std::string halfExtentString = GetAttribute<std::string>(node, "halfExtents", "1,1,1");
		
		std::vector<F32> values;
		ParseList(halfExtentString, ',', values);

		m_HalfExtentVector = Vector3<F32>(values.data());
	}

	BoxPhysicsComponent::BoxPhysicsComponent(const BoxPhysicsComponent & other)
		:	BulletPhysicsComponent(other),
			m_HalfExtentVector(other.m_HalfExtentVector)
	{}

	BoxPhysicsComponent::~BoxPhysicsComponent()
	{}

	AttachableComponent * BoxPhysicsComponent::Clone() const
	{
		return new BoxPhysicsComponent(*this);
	}

	void BoxPhysicsComponent::InitializeBulletComponent(BulletComponentImplementation * component)
	{
		component->shape = new btBoxShape(ToBulletVector(m_HalfExtentVector));

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), ToBulletVector(GetWorldPosition())));

		F32 mass = 1.0f;

		btVector3 inertia;
		component->shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo ci(mass, motionState, component->shape, inertia);

		component->rigidBody = new btRigidBody(ci);

		BulletPhysicsComponent::InitializeBulletComponent(component);
	}
}