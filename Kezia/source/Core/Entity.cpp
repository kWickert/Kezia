#include "Entity.h"

#include "Core/AttachmentTarget.h"
#include "Core/TransformationNode.h"
#include "Core/World.h"
#include "Core/XmlObjectBuilder.h"

#include "Graphics/RenderableComponentBuilder.h"

#include "Physics/BulletComponentBuilder.h"

namespace Kezia
{
	REGISTER_ENTITY_TYPE_IMPL(Entity);

	XmlObjectBuilder * Entity::k_ComponentBuilder;

	Entity::Entity()
		:	m_ID(INVALID_ENTITY_ID),
			m_Node(new TransformationNode()),
			m_WantsToDie(false)
	{}

	Entity::Entity(const XmlNode & node)
		:	m_ID(INVALID_ENTITY_ID),
			m_Node(new TransformationNode()),
			m_WantsToDie(false)
	{
		static bool componentBuilderInitResult = InitializeComponentBuilder();
	
		node.ValidateChildElements("Components", "");
		node.ValidateAttributes("name", "", false);

		XmlNode componentsNode = node.GetFirstChildNode();

		componentsNode.ValidateChildElements("", k_ComponentBuilder->GetRegisteredBuildersCSV());

		XmlNode componentNode = componentsNode.GetFirstChildNode();

		SetAttachmentTarget(m_Node);

		while(!componentNode.IsEmpty())
		{
			k_ComponentBuilder->BuildObject(componentNode);

			componentNode = componentNode.GetNextSiblingNode();
		}

		ClearAttachmentTarget();
	}

	Entity::Entity(const Entity & other)
		:	m_ID(INVALID_ENTITY_ID),
			m_Node(other.m_Node->FullCopy()),
			m_WantsToDie(false)
	{}

	Entity::~Entity()
	{
		g_World->NotifyDestroyEntity(this);
	}

	Entity * Entity::Clone() const
	{
		Entity * e = new Entity(*this);

		return e;
	}

	void Entity::Update(const F64 deltaTime)
	{}

	void Entity::Destroy()
	{
		m_WantsToDie = true;
	}

	bool Entity::InitializeComponentBuilder()
	{
		k_ComponentBuilder = new XmlObjectBuilder();

		XmlObjectBuilder * renderableBuilder = CreateOrGetRenderableComponentBuilder();
		renderableBuilder->DisableLogging();
		k_ComponentBuilder->RegisterChildBuilder(renderableBuilder);

		XmlObjectBuilder * bulletBuilder = CreateOrGetBulletComponentBuilder();
		bulletBuilder->DisableLogging();
		k_ComponentBuilder->RegisterChildBuilder(bulletBuilder);

		return true;
	}
}