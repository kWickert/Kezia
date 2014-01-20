#pragma once

#include "Core/Entity.h"
#include "Core/EntityTypes.h"

#include "Math/Vector3.h"

namespace Kezia
{
	class Camera;
	class PhysicsComponent;
	class XmlNode;

	class Pawn : public Entity
	{
	public:
		REGISTER_ENTITY_TYPE_DECL(Pawn);

		Pawn();
		Pawn(const XmlNode & node);
		Pawn(const Pawn & other);
		virtual ~Pawn();

		virtual Pawn * Clone() const;

		void ApplyImpulse(const Vector3<F32> & impulse);
		void SetLinearVelocity(const Vector3<F32> & velocity);
		
		void AttachCamera(Camera * camera);
		Camera * GetAttachedCamera();

		void SetPhysicsComponent(PhysicsComponent * physicsComp);

		const F32 GetMoveSpeed() const;
		void SetMoveSpeed(const F32 & speed);
	
		friend class World;
	protected:
		

	private:
		PhysicsComponent * m_PhysicsComponent;

		F32 m_MoveSpeed;

		Camera * m_Camera;
	};
}