#include "PhysicsManager.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

#include "Physics/PhysicsComponent.h"

#include <algorithm>

namespace Kezia
{
	PhysicsManager::~PhysicsManager()
	{
		std::for_each(m_Components.begin(), m_Components.end(), [&](PhysicsComponent * component)
			{
				delete component;
			});

		m_Components.clear();
	}

	void PhysicsManager::Update(const F64 deltaTime)
	{}

	bool PhysicsManager::AddPhysicsComponent(PhysicsComponent * component)
	{
		auto findResult = std::find(m_Components.begin(), m_Components.end(), component);

		bool isUnique = findResult == m_Components.end();
		bool isAttached = component->IsAttached();
		bool success = isUnique && isAttached;

		if(success)
		{
			m_Components.push_back(component);
		}
		else
		{
			if(!isUnique)
				LOG("PhysicsComponent already added to manager");

			if(!isAttached)
				LOG("PhysicsComponent is not attached to a node!");
		}

		return success;
	}

	void PhysicsManager::RemovePhysicsComponent(PhysicsComponent * component)
	{
		auto findResult = std::find(m_Components.begin(), m_Components.end(), component);

		KeziaAssert(findResult != m_Components.end());

		m_Components.erase(findResult);
	}
	
	void PhysicsManager::EnableDebugDrawing()
	{
		
	}

	void PhysicsManager::DisableDebugDrawing()
	{

	}
}