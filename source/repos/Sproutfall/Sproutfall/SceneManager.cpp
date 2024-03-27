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

	m_LeftBorder = make_unique<sf::RectangleShape>();
	m_LeftBorder->setFillColor(sf::Color::Blue);

	m_RightBorder = make_unique<sf::RectangleShape>();
	m_RightBorder->setFillColor(sf::Color::Red);

	m_UpperBorder = make_unique<sf::RectangleShape>();
	m_UpperBorder->setFillColor(sf::Color::Yellow);

	m_LowerBorder = make_unique<sf::RectangleShape>();
	m_LowerBorder->setFillColor(sf::Color::Green);

	m_LeftBorder->setSize(sf::Vector2f(100, 100));
	m_UpperBorder->setSize(sf::Vector2f(100, 100));
	m_RightBorder->setSize(sf::Vector2f(100, 100));
	m_LowerBorder->setSize(sf::Vector2f(100, 100));
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
		if (m_Player->getPosition().y < (m_Scene->getLevelSize()) - (m_viewSizeY / 2))
			m_Scene->Update(tf);
		m_Player->Update(tf);
		m_EnemyManager->Update(tf);

		m_View->setCenter(m_viewSizeX / 2, m_Player->getPosition().y);

		if (m_Player->getPosition().y >= (m_Scene->getLevelSize()))
		{
			loadScene(m_CurrentScene + 1);
		}
	}
	else if (m_smokeAnimationManager->isPlaying())
	{
		m_smokeAnimationManager->Update(tf);
	}

	m_UpperBorder->setPosition(m_View->getCenter().x - (m_View->getSize().x / 2), m_View->getCenter().y - (m_View->getSize().y / 2));
	m_LeftBorder->setPosition(m_View->getCenter().x - (m_View->getSize().x / 2), m_View->getCenter().y - (m_View->getSize().y / 2));

	m_LowerBorder->setPosition(m_View->getCenter().x - (m_View->getSize().x / 2), m_View->getCenter().y + (m_View->getSize().y / 2) - m_LowerBorder->getSize().y);
	m_RightBorder->setPosition(m_View->getCenter().x + (m_View->getSize().x / 2) - m_LowerBorder->getSize().x, m_View->getCenter().y - (m_View->getSize().y / 2));
}

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_UpperBorder.get());
	target.draw(*m_LowerBorder.get());
	target.draw(*m_RightBorder.get());
	target.draw(*m_LeftBorder.get());
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
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 10 * 960);

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


void SceneManager::borderView(int width, int height)
{

}

void SceneManager::handleResize(int width, int height)
{
	int basis;
	int normalizedWidth = width / 4;
	int normalizedHeight = height / 3;
	if (normalizedWidth == normalizedHeight)
	{
		m_View->setSize(m_viewSizeX, m_viewSizeY);
	}
	else if (normalizedWidth > normalizedHeight)
	{
		m_View->setSize(m_viewSizeX * normalizedWidth / normalizedHeight, m_viewSizeY);
	}
	else
	{
		m_View->setSize(m_viewSizeX, m_viewSizeY * normalizedHeight / normalizedWidth);
	}
	//If we hit bugs, update m_viewsizex and m_viewsizey
}