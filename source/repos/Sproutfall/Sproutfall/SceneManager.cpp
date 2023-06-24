#include "SFML_Lib.h"
#include "SceneManager.h"

SceneManager::SceneManager(Player* player, float viewSizeX, float viewSizeY)
{
	m_Player = player;
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_Scene = new Scene(player, m_viewSizeX, m_viewSizeY);
	m_View = new sf::View();
	m_View->setSize(viewSizeX, viewSizeY);
}
SceneManager::~SceneManager()
{
	delete(m_Scene);
	delete(m_View);
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
	delete(m_Scene);
	m_Scene = new Scene(m_Player, m_viewSizeX, m_viewSizeY, 10);

	sf::Texture* farStarsTexture = new sf::Texture();
	if (!farStarsTexture->loadFromFile("Textures/space_stars_small.png"))
	{
		cout << "space_stars_big.png failed to load!" << endl;
	}
	m_Scene->addBackground(0, farStarsTexture);

	sf::Texture* closeStarsTexture = new sf::Texture();
	if (!closeStarsTexture->loadFromFile("Textures/space_stars_big.png"))
	{
		cout << "space_stars_small.png failed to load!" << endl;
	}
	m_Scene->addBackground(0.5, closeStarsTexture);

	sf::Texture* backgroundObjectTexture = new sf::Texture();
	if (!backgroundObjectTexture->loadFromFile("Textures/background_objects.png"))
	{
		cout << "background_objects.png failed to load!" << endl;
	}
	m_Scene->addBackground(1, backgroundObjectTexture);

	sf::Texture* earthTexture = new sf::Texture();
	if (!earthTexture->loadFromFile("Textures/earth.png"))
	{
		cout << "earth.png failed to load!" << endl;
	}
	m_Scene->addBackground(1, earthTexture, 9);

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
