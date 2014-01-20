#pragma once

#include "Core/Blueprint.h"
#include "Core/XmlObjectBuilder.h"

#include <vector>

namespace Kezia
{
	class XmlNode;
	class XmlObjectBuilder;

	template<typename Entity_T>
	void * BuildBlueprint(const XmlNode & blueprintNode)
	{
		return new Blueprint<Entity_T>(new Entity_T(blueprintNode));
	}

	struct EntityTypeInfo
	{
		static std::vector<EntityTypeInfo *> & GetTypes();

		virtual void AddToXmlBuilder(XmlObjectBuilder * builder) = 0;
	};

	template<typename Entity_T>
	struct EntityType : public EntityTypeInfo
	{
		EntityType(const std::string & typeName)
			:	typeName(typeName)
		{
			std::cout << typeName << std::endl;
			GetTypes().push_back(this);
		}

		virtual void AddToXmlBuilder(XmlObjectBuilder * builder)
		{
			builder->RegisterObjectBuilder(typeName, BuildBlueprint<Entity_T>);
		}
	
		std::string typeName;
	};

	#define REGISTER_ENTITY_TYPE_DECL(entityType) static const EntityType<##entityType> Type;
	#define REGISTER_ENTITY_TYPE_IMPL(entityType) const EntityType<##entityType> ##entityType::Type(#entityType);
}