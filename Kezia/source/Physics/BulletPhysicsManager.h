#pragma once

#include "Physics/PhysicsManager.h"

#include <map>
#include <vector>

namespace Kezia
{
	class BulletDebugRenderer;

	struct BulletComponentImplementation;
	struct BulletManagerImplementation;

	class BulletPhysicsManager : public PhysicsManager
	{
	public:
		BulletPhysicsManager();
		virtual ~BulletPhysicsManager();

		virtual void Update(const F64 deltaTime);

		virtual bool AddPhysicsComponent(PhysicsComponent * component);
		virtual void RemovePhysicsComponent(PhysicsComponent * component);

		virtual void EnableDebugDrawing();
		virtual void DisableDebugDrawing();

		virtual void SetGravity(const Vector3<F32> & gravitationalForce);
	private:
		BulletManagerImplementation * m_ManagerImplementation;

		BulletDebugRenderer * m_DebugRenderer;
	};
}