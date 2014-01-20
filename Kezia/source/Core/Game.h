#pragma once

#include <string>
#include <vector>

namespace Kezia
{
	class Level;
	class XmlObjectBuilder;

	class Game
	{
	public:
		Game();
		virtual ~Game();

		void LoadLevel(const std::string & levelName);

	private:
		std::vector<Level *> m_Levels;
	};
}