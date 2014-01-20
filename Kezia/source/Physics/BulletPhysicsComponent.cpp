#include "BulletPhysicsComponent.h"

#include "Physics/BulletUtility.h"
#include "Physics/PhysicsManager.h"

#include <btBulletDynamicsCommon.h>

namespace Kezia
{
	BulletPhysicsComponent::BulletPhysicsComponent()
	{}

	BulletPhysicsComponent::BulletPhysicsComponent(const BulletPhysicsComponent & other)
		:	PhysicsComponent(other)
	{}

	BulletPhysicsComponent::~BulletPhysicsComponent()
	{
		delete m_ComponentImplementation;
	}

	AttachableComponent * BulletPhysicsComponent::Clone() const
	{
		return new BulletPhysicsComponent(*this);
	}

	void BulletPhysicsComponent::PostAttach()
	{
		g_PhysicsManager->AddPhysicsComponent(this);
	}

	void BulletPhysicsComponent::NotifyTransformUpdate()
	{
		//some property was externally modified
		Matrix4<F32> worldTransform = m_Node->GetWorldTransformNoScale();

		const Vector3<F32> position = ExtractTranslationComponent(worldTransform);
		const Matrix3<F32> orientation = ExtractOrientationComponent(worldTransform);

		//m_ComponentImplementation->rigidBody->setMotionState(new btDefaultMotionState(btTransform(ToBulletMatrix(orientation), ToBulletVector(position))));

		btTransform bulletTransform = btTransform(ToBulletMatrix(orientation), ToBulletVector(position));
		m_ComponentImplementation->rigidBody->setWorldTransform(bulletTransform);
		m_ComponentImplementation->rigidBody->setCenterOfMassTransform(bulletTransform);
	}

	void BulletPhysicsComponent::Update(const F64 deltaTime)
	{
		if(m_ComponentImplementation != nullptr && m_Node != nullptr)
		{
			btTransform transform;

			m_ComponentImplementation->rigidBody->getMotionState()->getWorldTransform(transform);

			m_Node->SetPosition(FromBulletVector(transform.getOrigin()));
			m_Node->SetOrientation(FromBulletQuaternion(transform.getRotation()));
		}
	}

	void BulletPhysicsComponent::ApplyImpulse(const Vector3<F32> & impulse)
	{
		m_ComponentImplementation->rigidBody->applyCentralImpulse(ToBulletVector(impulse));
	}

	void BulletPhysicsComponent::SetLinearVelocity(const Vector3<F32> & velocity)
	{
		m_ComponentImplementation->rigidBody->setLinearVelocity(ToBulletVector(velocity));
	}

	void BulletPhysicsComponent::InitializeBulletComponent(BulletComponentImplementation * component)
	{
		m_ComponentImplementation = component;
	}

	void BulletPhysicsComponent::EnableKinematicPhysics()
	{
		btRigidBody * body = m_ComponentImplementation->rigidBody;

		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	void BulletPhysicsComponent::EnablePawnPhysics()
	{
		btRigidBody * body = m_ComponentImplementation->rigidBody;
		
		body->setSleepingThresholds(0, 0);
		body->setAngularFactor(0);
	}
}