#include "SFML_Lib.h"
#include "SceneManager.h"

SceneManager::SceneManager(Player* player, float viewSizeX, float viewSizeY)
{
	m_Player = player;
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_Scene = make_unique<Scene>(player, m_viewSizeX, m_viewSizeY);
	m_View = make_unique<sf::View>();
	m_View->setSize(viewSizeX, viewSizeY);
}
SceneManager::~SceneManager()
{

}
void SceneManager::loadScene(int scene)
{
	if (scene == TitleScreen)
	{
		loadTitle();
	}
	else if (scene == Space)
	{
		loadSpace();
	}
	else if (scene == Sky)
	{
		loadSky();
	}
	else if (scene == Ground)
	{
		loadGround();
	}
	else if (scene == Win)
	{
		loadWin();
	}
}
void SceneManager::Update(float tf)
{
	m_Scene->Update(tf);
	m_View->setCenter(640, m_Player->getPosition().y);
}

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_Player->getPosition().y < (m_Scene->getLevelSize() * m_viewSizeY) - (m_viewSizeY / 2))
	{
		if (!m_Scene->getParallax())
		{
			m_Scene->setParallax(true);
		}
		target.setView(*m_View);
	}
	else
	{
		if (m_Scene->getParallax())
		{
			m_Scene->setParallax(false);
		}
	}
	target.draw(*m_Scene);
	target.draw(*m_Player);
}

void SceneManager::loadTitle()
{

}

void SceneManager::loadSpace()
{
	
	m_Scene.reset(new Scene(m_Player, m_viewSizeX, m_viewSizeY, 2));

	m_Scene->addBackground(1, "Textures/space_stars_small.png");

	m_Scene->addBackground(0.5, "Textures/space_stars_big.png");

	m_Scene->addBackground(0, "Textures/background_objects.png");

	m_Scene->addBackground(-0.99, "Textures/earth.png", -1);

	m_Scene->setBackgroundFillColor(0x655057ff);
}

void SceneManager::loadSky()
{

}

void SceneManager::loadGround()
{

}

void SceneManager::loadWin()
{

}
