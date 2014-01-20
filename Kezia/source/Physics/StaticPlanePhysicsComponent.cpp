#include "StaticPlanePhysicsComponent.h"

#include "Physics/BulletUtility.h"

#include "Core/StringUtilities.h"
#include "Core/XmlNode.h"

namespace Kezia
{
	StaticPlanePhysicsComponent::StaticPlanePhysicsComponent(const XmlNode & node)
	{
		node.ValidateAttributes("normal,displacement", "");

		std::string normalStr = GetAttribute<std::string>(node, "normal", "0,0,0");

		std::vector<F32> normalEles;
		ParseList(normalStr, ',', normalEles);

		m_PlaneNormal = Vector3<F32>(normalEles.data());
		
		m_Displacement = GetAttribute(node, "displacement", 0.0f);
	}

	StaticPlanePhysicsComponent::StaticPlanePhysicsComponent(const Vector3<F32> & planeNormal, const F32 displacement)
		:	m_PlaneNormal(planeNormal),
			m_Displacement(displacement)
	{}

	StaticPlanePhysicsComponent::StaticPlanePhysicsComponent(const StaticPlanePhysicsComponent & other)
		:	BulletPhysicsComponent(other),
			m_PlaneNormal(other.m_PlaneNormal),
			m_Displacement(other.m_Displacement)
	{}

	StaticPlanePhysicsComponent::~StaticPlanePhysicsComponent()
	{}

	AttachableComponent * StaticPlanePhysicsComponent::Clone() const
	{
		return new StaticPlanePhysicsComponent(*this);
	}

	void StaticPlanePhysicsComponent::InitializeBulletComponent(BulletComponentImplementation * component)
	{
		component->shape = new btStaticPlaneShape(ToBulletVector(m_PlaneNormal), m_Displacement);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo ci(0, motionState, component->shape, btVector3(0, 0, 0));

		component->rigidBody = new btRigidBody(ci);

		BulletPhysicsComponent::InitializeBulletComponent(component);
	}
}