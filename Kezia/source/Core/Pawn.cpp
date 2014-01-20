#include "Pawn.h"

#include "Core/XmlNode.h"

#include "Graphics/Camera.h"

#include "Physics/PhysicsComponent.h"

namespace Kezia
{
	REGISTER_ENTITY_TYPE_IMPL(Pawn);

	Pawn::Pawn()
		:	m_Camera(nullptr),
			m_PhysicsComponent(nullptr)
	{}

	Pawn::Pawn(const XmlNode & node)
		:	Entity(node)
	{
		node.ValidateAttributes("moveSpeed", "", false);

		m_MoveSpeed = GetAttribute(node, "moveSpeed", 50.0f);

		for(auto it = m_Node->ComponentBegin(); it != m_Node->ComponentEnd(); ++it)
		{
			PhysicsComponent * physicsComp = dynamic_cast<PhysicsComponent *>(*it);

			if(physicsComp)
			{
				SetPhysicsComponent(physicsComp);
				break;
			}
		}
	}

	Pawn::Pawn(const Pawn & other)
		:	Entity(other),
			m_MoveSpeed(other.m_MoveSpeed),
			m_Camera(nullptr)
	{
		for(auto it = m_Node->ComponentBegin(); it != m_Node->ComponentEnd(); ++it)
		{
			PhysicsComponent * physicsComp = dynamic_cast<PhysicsComponent *>(*it);

			if(physicsComp)
			{
				SetPhysicsComponent(physicsComp);
				break;
			}
		}
	}

	Pawn::~Pawn()
	{}

	Pawn * Pawn::Clone() const
	{
		return new Pawn(*this);
	}

	void Pawn::ApplyImpulse(const Vector3<F32> & impulse)
	{
		if(m_PhysicsComponent != nullptr && impulse.GetLengthSquared() != 0)
			m_PhysicsComponent->ApplyImpulse(impulse);
	}

	void Pawn::SetLinearVelocity(const Vector3<F32> & velocity)
	{
		if(m_PhysicsComponent != nullptr)
			m_PhysicsComponent->SetLinearVelocity(velocity);
	}

	void Pawn::SetPhysicsComponent(PhysicsComponent * physicsComp)
	{
		m_PhysicsComponent = physicsComp;
		m_PhysicsComponent->Attach(m_Node);
		m_PhysicsComponent->EnablePawnPhysics();
		//m_PhysicsComponent->EnableKinematicPhysics();
	}

	void Pawn::AttachCamera(Camera * camera)
	{
		m_Camera = camera;
		m_Camera->Attach(m_Node);
	}

	Camera * Pawn::GetAttachedCamera()
	{
		return m_Camera;
	}

	const F32 Pawn::GetMoveSpeed() const
	{
		return m_MoveSpeed;
	}

	void Pawn::SetMoveSpeed(const F32 & speed)
	{
		m_MoveSpeed = speed;
	}
}