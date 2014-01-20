#pragma once

#include "Core/EntityTypes.h"
#include "Core/Types.h"

#include "Math/Matrix3.h"
#include "Math/Vector3.h"

#include <vector>

#define INVALID_ENTITY_ID -1

namespace Kezia
{
	class TransformationNode;
	class World;
	class XmlNode;
	class XmlObjectBuilder;

	typedef S32 EntityID;

	class Entity
	{
	public:
		REGISTER_ENTITY_TYPE_DECL(Entity);

		Entity(const XmlNode & node);
		virtual Entity * Clone() const;

		virtual void Update(const F64 deltaTime);

		void Destroy();

		inline EntityID GetEntityID() const;
		inline TransformationNode * GetTransformationNode();

		friend class World;
	protected:
		Entity();
		
		Entity(const Entity & other);
		virtual ~Entity();

		std::string m_Name;

		EntityID m_ID;

		bool m_WantsToDie;

		TransformationNode * m_Node;
	private:
		static bool InitializeComponentBuilder();
		static XmlObjectBuilder * k_ComponentBuilder;

		void AssembleComponents();
	};

	inline EntityID Entity::GetEntityID() const
	{
		return m_ID;
	}

	inline TransformationNode * Entity::GetTransformationNode()
	{
		return m_Node;
	}
}