#include "World.h"

#include "Core/Assert.h"
#include "Core/AttachableComponent.h"
#include "Core/AttachmentTarget.h"
#include "Core/Entity.h"
#include "Core/EntityTypes.h"
#include "Core/FileResource.h"
#include "Core/Logger.h"
#include "Core/Pawn.h"
#include "Core/TransformationNode.h"
#include "Core/XmlNode.h"
#include "Core/XmlObjectBuilder.h"
#include "Core/Windows_ResourceManager.h"

#include "Event/Event.h"

#include "Graphics/RenderableComponentBuilder.h"

#include "Physics/BulletComponentBuilder.h"

namespace Kezia
{
	World::World()
	{
		m_BlueprintBuilder = new XmlObjectBuilder();
		InitializeBlueprintBuilder(m_BlueprintBuilder);

		std::vector<FileResource *> entityDefinitions;

		FileResource * file = reinterpret_cast<FileResource *>(g_FileResourceManager->OpenResourceFile("entities.xml"));

		XmlNode root(file->GetFilePath());

		m_Entities.reserve(1000);

		XmlNode entityNode = root.GetFirstChildNode().GetFirstChildNode();
		while(!entityNode.IsEmpty())
		{
			BlueprintBase * blueprint = reinterpret_cast<BlueprintBase *>(m_BlueprintBuilder->BuildObject(entityNode));

			std::string blueprintName = GetAttribute<std::string>(entityNode, "name", "");
			m_EntityBlueprints.Insert(blueprintName, blueprint);

			entityNode = entityNode.GetNextSiblingNode();
		}	
	}

	World::~World()
	{
		for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
		{
			delete *it;
		}
	}

	void World::Update(const F64 deltaTime)
	{
		while(m_NewEntities.size() > 0)
		{
			NamedProperties args;
			args.SetProperty("entityID", m_NewEntities.front()->GetEntityID());

			FireEvent("entitySpawned", args);

			m_NewEntities.pop_front();			
		}

		for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
		{
			Entity * e = *it;

			if(e != nullptr)
			{
				e->Update(deltaTime);

				if(e->m_WantsToDie)
				{
					delete e;
					e = nullptr;
				}
			}			
		}
	}

	void World::InitializeBlueprintBuilder(XmlObjectBuilder * blueprintBuilder)
	{
		auto types = EntityTypeInfo::GetTypes();

		for(auto it = types.begin(); it != types.end(); ++it)
		{
			(*it)->AddToXmlBuilder(blueprintBuilder);
		}
	}

	void World::Spawn(const std::string & blueprintName, const Vector3<F32> & location)
	{
		auto findResult = m_EntityBlueprints.Find(blueprintName);
		if(findResult != m_EntityBlueprints.End())
		{
			BlueprintBase * blueprint = findResult->second;

			Entity * newEntity = reinterpret_cast<Entity *>(blueprint->CreateUntypedInstance());

			if(newEntity != nullptr)
			{
				newEntity->GetTransformationNode()->SetPosition(location);
				RegisterEntity(newEntity, -1);
			}
		}
	}

	Entity * World::GetEntity(const U32 entityID)
	{
		Entity * e = nullptr;

		if(entityID < m_Entities.size())
			e = m_Entities[entityID];

		if(e == nullptr)
		{
			LOG("entity with ID " << entityID << " does not exist!");
		}

		return e;
	}

	const U32 World::GetEntityCount() const
	{
		return m_Entities.size();
	}

	void World::RegisterEntityBlueprint(const std::string & entityType, BlueprintBase * entityBlueprint)
	{
		auto findResult = m_EntityBlueprints.Find(entityType);

		if(findResult != m_EntityBlueprints.End())
			m_EntityBlueprints.Insert(entityType, entityBlueprint);
		else
			LOG("blueprint already exists for entityType: " << entityType);
	}

	const U32 World::RegisterEntity(Entity * entity, S32 requestedID)
	{
		U32 id = m_Entities.size();
		bool duplicateFound = false;

		if(requestedID >= 0)
		{
			if(static_cast<U32>(requestedID) >= m_Entities.size() || m_Entities[requestedID] == nullptr)
			{
				id = requestedID;			
			}
			else if(m_Entities[requestedID] != nullptr)
			{
				duplicateFound = true;
			}
		}

		if(id >= m_Entities.size())
		{
			m_Entities.resize(id + 1, nullptr);
		}
		
		m_Entities[id] = entity;
		entity->m_ID = id;

		if(!duplicateFound)
		{
			m_NewEntities.push_back(entity);
		}
		else
		{
			LOG("entity with id, " << requestedID << ", already exists!");
			//KeziaAssert(false);
		}
		
		return id;
	}

	void World::NotifyDestroyEntity(Entity * entity)
	{
		if(entity != nullptr)
		{
			const U32 id = entity->GetEntityID();

			if(id != INVALID_ENTITY_ID)
			{
				NamedProperties args;
				args.SetProperty("entityID", id);

				FireEvent("entityDestroyed", args);
			}
		}
	}
}