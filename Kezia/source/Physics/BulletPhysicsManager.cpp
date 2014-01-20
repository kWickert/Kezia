#include "BulletPhysicsManager.h"

#include "Core/Logger.h"

#include "Physics/BulletDebugRenderer.h"
#include "Physics/BulletPhysicsComponent.h"
#include "Physics/BulletUtility.h"

#include <btBulletDynamicsCommon.h>

namespace Kezia
{
	BulletPhysicsManager::BulletPhysicsManager()
	{
		m_ManagerImplementation = new BulletManagerImplementation();
		m_DebugRenderer = new BulletDebugRenderer(m_ManagerImplementation);

		m_ManagerImplementation->broadphase = new btDbvtBroadphase();

		m_ManagerImplementation->collisionConfiguration = new btDefaultCollisionConfiguration();
		m_ManagerImplementation->dispatcher = new btCollisionDispatcher(m_ManagerImplementation->collisionConfiguration);

		m_ManagerImplementation->solver = new btSequentialImpulseConstraintSolver();

		m_ManagerImplementation->dynamicsWorld = new btDiscreteDynamicsWorld(m_ManagerImplementation->dispatcher, m_ManagerImplementation->broadphase, m_ManagerImplementation->solver, m_ManagerImplementation->collisionConfiguration);
		m_ManagerImplementation->dynamicsWorld->setDebugDrawer(m_DebugRenderer);

		SetGravity(Vector3<F32>(0, -10, 0));
	}

	BulletPhysicsManager::~BulletPhysicsManager()
	{
		//delete shapes, rigid body, and motion state

		delete m_ManagerImplementation;

		delete m_DebugRenderer;
	}

	void BulletPhysicsManager::Update(const F64 deltaTime)
	{
		m_ManagerImplementation->dynamicsWorld->stepSimulation(static_cast<F32>(deltaTime), 10);

		for(auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}
	}

	bool BulletPhysicsManager::AddPhysicsComponent(PhysicsComponent * component)
	{
		BulletPhysicsComponent * btComp = dynamic_cast<BulletPhysicsComponent *>(component);

		bool isBulletCompatible = btComp != nullptr;
		bool success = isBulletCompatible && PhysicsManager::AddPhysicsComponent(component);

		if(success)
		{
			BulletComponentImplementation * cImpl = new BulletComponentImplementation();

			btComp->InitializeBulletComponent(cImpl);

			m_ManagerImplementation->dynamicsWorld->addRigidBody(cImpl->rigidBody);
		}
		else
		{
			if(!isBulletCompatible)
				LOG("PhysicsComponent does not implement BulletPhysicsComponent interface");
		}

		return success;		
	}

	void BulletPhysicsManager::RemovePhysicsComponent(PhysicsComponent * component)
	{
		BulletPhysicsComponent * btComp = dynamic_cast<BulletPhysicsComponent *>(component);

		bool isBulletCompatible = btComp != nullptr;
		bool success = isBulletCompatible;
		
		if(success)
		{
			PhysicsManager::RemovePhysicsComponent(component);

			m_ManagerImplementation->dynamicsWorld->removeRigidBody(btComp->m_ComponentImplementation->rigidBody);
		}
	}

	void BulletPhysicsManager::EnableDebugDrawing()
	{
		m_DebugRenderer->SetVisibility(true);
	}

	void BulletPhysicsManager::DisableDebugDrawing()
	{
		m_DebugRenderer->SetVisibility(false);
	}

	void BulletPhysicsManager::SetGravity(const Vector3<F32> & gravitationalForce)
	{
		m_ManagerImplementation->dynamicsWorld->setGravity(ToBulletVector(gravitationalForce));

		PhysicsManager::SetGravity(gravitationalForce);
	}
}