#include "CapsulePhysicsComponent.h"

#include "Core/XmlNode.h"

#include "Physics/BulletUtility.h"

namespace Kezia
{
	CapsulePhysicsComponent::CapsulePhysicsComponent(const XmlNode & node)
	{
		node.ValidateAttributes("radius,height","");

		m_Radius = GetAttribute(node, "radius", 0.0f);

		m_Height = GetAttribute(node, "height", 0.0f);
	}

	CapsulePhysicsComponent::CapsulePhysicsComponent(const CapsulePhysicsComponent & other)
		:	BulletPhysicsComponent(other),
			m_Radius(other.m_Radius),
			m_Height(other.m_Height)
	{}

	CapsulePhysicsComponent::~CapsulePhysicsComponent()
	{}

	AttachableComponent * CapsulePhysicsComponent::Clone() const
	{
		return new CapsulePhysicsComponent(*this);
	}

	void CapsulePhysicsComponent::InitializeBulletComponent(BulletComponentImplementation * component)
	{
		component->shape = new btCapsuleShape(m_Radius, m_Height);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), ToBulletVector(GetWorldPosition())));

		F32 mass = 1.0f;

		btVector3 inertia;
		component->shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo ci(mass, motionState, component->shape, inertia);

		component->rigidBody = new btRigidBody(ci);

		BulletPhysicsComponent::InitializeBulletComponent(component);
	}
}