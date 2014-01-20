#include "Game.h"

#include "Core/Level.h"
#include "Core/XmlNode.h"
#include "Core/XmlObjectBuilder.h"
#include "Core/Windows_ResourceManager.h"

namespace Kezia
{
	Game::Game()
	{}

	Game::~Game()
	{}

	void Game::LoadLevel(const std::string & levelName)
	{
		std::string levelFile = levelName + ".xml";

		if(g_FileResourceManager->FileExists(levelFile))
		{
			m_Levels.push_back(new Level(XmlNode(g_FileResourceManager->GetFilePath(levelFile))));
		}
	}
}