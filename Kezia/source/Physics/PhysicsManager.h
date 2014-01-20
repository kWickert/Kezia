#pragma once

#include "Core/Types.h"

#include "Math/Vector3.h"

#include <vector>

namespace Kezia
{
	class PhysicsComponent;

	class PhysicsManager
	{
	public:
		virtual ~PhysicsManager();

		virtual void Update(const F64 deltaTime);
		
		virtual bool AddPhysicsComponent(PhysicsComponent * component);
		virtual void RemovePhysicsComponent(PhysicsComponent * component);

		virtual void EnableDebugDrawing();
		virtual void DisableDebugDrawing();

		virtual inline void SetGravity(const Vector3<F32> & gravitationalForce);
		virtual inline const Vector3<F32> & GetGravity() const;
	protected:
		std::vector<PhysicsComponent *> m_Components;

		Vector3<F32> m_Gravity;
	};

	extern PhysicsManager * g_PhysicsManager;

	void PhysicsManager::SetGravity(const Vector3<F32> & gravitationalForce)
	{
		m_Gravity = gravitationalForce;
	}

	const Vector3<F32> & PhysicsManager::GetGravity() const
	{
		return m_Gravity;
	}
}