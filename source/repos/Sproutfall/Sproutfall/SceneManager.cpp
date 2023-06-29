#include "SFML_Lib.h"
#include "SceneManager.h"

SceneManager::SceneManager(float viewSizeX, float viewSizeY, sf::RenderWindow* window)
{
	m_Player = make_unique<Player>(window);
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY);
	m_View = make_unique<sf::View>();
	m_View->setSize(viewSizeX, viewSizeY);
	m_EnemyManager = make_unique<EnemyManager>(m_View->getSize(), m_Player.get());
	m_playerSmoke = make_unique<sf::Sprite>();
	m_smokeTexture = make_unique<sf::Texture>();
	if (!m_smokeTexture->loadFromFile("Textures/playerSmoke.png"))
	{
		cout << "Player smoke texture load failure" << endl;
	}
	m_playerSmoke->setTexture(*m_smokeTexture);
	m_smokeAnimationManager = make_unique<AnimationManager>(m_playerSmoke.get());
	vector<sf::IntRect> frameVector;
	frameVector.push_back(sf::IntRect(0, 0, 19, 17));
	frameVector.push_back(sf::IntRect(19, 0, 19, 18));
	frameVector.push_back(sf::IntRect(38, 0, 21, 20));
	frameVector.push_back(sf::IntRect(59, 0, 26, 23));
	frameVector.push_back(sf::IntRect(85, 0, 28, 25));
	frameVector.push_back(sf::IntRect(113, 0, 30, 27));
	frameVector.push_back(sf::IntRect(143, 0, 30, 27));
	m_playerSmoke->setOrigin(12, 15);
	m_playerSmoke->setScale(2, 2);
	m_smokeAnimationManager->addState(0, frameVector, false, 0.07f);
	frameVector.clear();
	m_smokeAnimationManager->Play();
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
	if (m_Player->getStatus())
	{
		if (m_Player->getPosition().y < (m_Scene->getLevelSize() * m_viewSizeY) - (m_viewSizeY / 2))
			m_Scene->Update(tf);
		m_Player->Update(tf);
		m_EnemyManager->Update(tf);

		m_View->setCenter(640, m_Player->getPosition().y);

		if (m_Player->getPosition().y >= (m_Scene->getLevelSize() * m_viewSizeY))
		{
			loadScene(m_CurrentScene + 1);
		}
	}
	else if (m_smokeAnimationManager->isPlaying())
	{
		m_smokeAnimationManager->Update(tf);
	}
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
	target.draw(*m_Scene);
	target.draw(*m_EnemyManager);
	if (m_Player->getStatus())
	{
		target.draw(*m_Player);
	}
	else if (m_smokeAnimationManager->isPlaying())
	{
		m_playerSmoke->setPosition(m_Player->getPosition());
		target.draw(*m_playerSmoke);
	}
}

void SceneManager::loadTitle()
{
	m_CurrentScene = TitleScreen;
}

void SceneManager::loadSpace()
{
	m_CurrentScene = Space;
	m_Scene.reset();
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 10);

	m_Scene->addBackground(-0.7, "Textures/space_stars_small.png");

	m_Scene->addBackground(-0.8, "Textures/space_stars_big.png");

	m_Scene->addBackground(-0.92, "Textures/background_objects.png");

	m_Scene->addBackground(-0.99, "Textures/earth.png", -1);

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_EnemyManager->generateEnemies(b_Squid, m_viewSizeY / 10, m_viewSizeY / 10, 9);

	m_EnemyManager->generateEnemies(b_Asteroid, m_viewSizeY / 10, m_viewSizeY / 10, 9);
}

void SceneManager::loadSky()
{
	m_CurrentScene = Sky;
	//m_Scene.reset();
	m_Player->setPosition(100, 100);
}

void SceneManager::loadGround()
{
	m_CurrentScene = Ground;
}

void SceneManager::loadWin()
{
	m_CurrentScene = Win;
}

void SceneManager::handleInput(sf::Event* event)
{
	m_Player->handleInput(event);
}
