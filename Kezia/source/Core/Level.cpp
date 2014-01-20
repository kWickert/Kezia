#include "Level.h"

#include "Core/Entity.h"
#include "Core/XmlNode.h"
#include "Core/World.h"

namespace Kezia
{
	Level::Level(const XmlNode & levelNode)
	{
		XmlNode entityNode = levelNode.GetFirstChildNode();

		while(!entityNode.IsEmpty())
		{
			std::string blueprintName = entityNode.GetName();

			levelNode.ValidateAttributes("location", "");

			g_World->Spawn(blueprintName, GetAttribute(entityNode, "location", Vector3<F32>()));
		}
	}

	Level::~Level()
	{}

	void Level::HideLevel()
	{
		for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
		{
			//(*it)->Hide();
		}
	}
}