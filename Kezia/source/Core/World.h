#pragma once

#include "Core/Blueprint.h"
#include "Core/Types.h"
#include "Core/HashedRegistry.h"

#include "Math/Vector3.h"

#include <list>
#include <vector>

namespace Kezia
{
	class Entity;
	class XmlNode;
	class XmlObjectBuilder;

	class World
	{
	public:
		World();
		virtual ~World();

		virtual void Update(const F64 deltaTime);

		virtual void InitializeBlueprintBuilder(XmlObjectBuilder * blueprintBuilder);
		
		void Spawn(const std::string & blueprintName, const Vector3<F32> & location);

		template<typename Actor_T>
		void Spawn(const std::string & blueprintName, const Vector3<F32> & location, Actor_T * & outActor);

		Entity * GetEntity(const U32 entityID);
		const U32 GetEntityCount() const;

		void RegisterEntityBlueprint(const std::string & entityType, BlueprintBase * entityBlueprint);

		friend class Entity;
	protected:
		template<typename Entity_T>
		static void * BuildBlueprint(const XmlNode & blueprintNode);

	private:
		XmlObjectBuilder * m_BlueprintBuilder;

		World(const World & other);

		const U32 RegisterEntity(Entity * entity, S32 requestedID);
		void NotifyDestroyEntity(Entity * entity);

		std::vector<Entity *> m_Entities;
		std::list<Entity *> m_NewEntities;

		HashedRegistry< BlueprintBase *> m_EntityBlueprints;
	};

	extern World * g_World;

	template<typename Actor_T>
	void World::Spawn(const std::string & blueprintName, const Vector3<F32> & location, Actor_T * & outActor)
	{
		outActor = nullptr;

		auto findResult = m_EntityBlueprints.Find(blueprintName);
		if(findResult != m_EntityBlueprints.End())
		{
			KeziaAssert(findResult->second->GetBlueprintType() == typeid(Actor_T));
			
			Blueprint<Actor_T> * blueprint = reinterpret_cast<Blueprint<Actor_T> *>(findResult->second);

			outActor = blueprint->CreateTypedInstance();
			outActor->GetTransformationNode()->SetPosition(location);

			RegisterEntity(outActor, INVALID_ENTITY_ID);
		}
	}

	template<typename Entity_T>
	void * World::BuildBlueprint(const XmlNode & blueprintNode)
	{
		return new Blueprint<Entity_T>(new Entity_T(blueprintNode));
	}
}