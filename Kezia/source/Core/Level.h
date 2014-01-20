#pragma once

#include <vector>

namespace Kezia
{
	class Entity;
	class XmlNode;

	class Level
	{
	public:
		Level(const XmlNode & levelNode);
		virtual ~Level();

		void HideLevel();

	private:
		std::vector<Entity *> m_Entities;
	};
}