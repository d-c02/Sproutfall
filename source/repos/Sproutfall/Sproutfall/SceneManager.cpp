#include "SFML_Lib.h"
#include "SceneManager.h"

SceneManager::SceneManager(float windowSizeX, float windowSizeY, sf::RenderWindow* window)
{
	m_renderWindow = window;
	m_Player = make_unique<Player>(window);
	//m_viewSizeX = viewSizeX;
	//m_viewSizeY = viewSizeY;
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY);
	m_View = make_unique<sf::View>();
	//m_View->setSize(m_viewSizeX, m_viewSizeY);
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
	m_playerSmoke->setTextureRect(sf::IntRect(0, 0, 19, 17));
	m_smokeAnimationManager->Play();

	m_LeftBorder = make_unique<sf::RectangleShape>();
	m_LeftBorder->setFillColor(sf::Color(0x655057ff));

	m_RightBorder = make_unique<sf::RectangleShape>();
	m_RightBorder->setFillColor(sf::Color(0x655057ff));

	m_UpperBorder = make_unique<sf::RectangleShape>();
	m_UpperBorder->setFillColor(sf::Color(0x655057ff));

	m_LowerBorder = make_unique<sf::RectangleShape>();
	m_LowerBorder->setFillColor(sf::Color(0x655057ff));

	m_UpperBorder->setSize(sf::Vector2f(m_ScreenShakeSizeX * 2 + m_viewSizeX, m_ScreenShakeSizeY));
	m_LeftBorder->setSize(sf::Vector2f(m_ScreenShakeSizeX, m_ScreenShakeSizeY + m_viewSizeY));
	m_LowerBorder->setSize(sf::Vector2f(m_ScreenShakeSizeX * 2 + m_viewSizeX, m_ScreenShakeSizeY));
	m_RightBorder->setSize(sf::Vector2f(m_ScreenShakeSizeX, m_ScreenShakeSizeY + m_viewSizeY));

	handleResize(windowSizeX, windowSizeY);
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

		sf::Vector2f playerPos;
		if (m_Player->getPosition().y < (m_Scene->getLevelSize()) - (m_viewSizeY / 2))
		{
			playerPos = m_Player->getPosition();
		}
		else
		{
			playerPos = sf::Vector2f(m_viewSizeX / 2, (m_Scene->getLevelSize()) - (m_viewSizeY / 2));
		}

		m_View->setCenter(m_viewSizeX / 2 + m_ScreenShakeOffset.x, playerPos.y + m_ScreenShakeOffset.y);
		m_UpperBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2) - m_ScreenShakeSizeY);
		m_LeftBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2) - m_ScreenShakeSizeY);
		m_LowerBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y + (m_View->getSize().y / 2) - m_LowerBorder->getSize().y + m_ScreenShakeSizeY);
		m_RightBorder->setPosition((m_viewSizeX / 2) + (m_View->getSize().x / 2) - m_RightBorder->getSize().x + m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2));

		if (m_Player->getPosition().y >= (m_Scene->getLevelSize()))
		{
			loadScene(m_CurrentScene + 1);
		}
	}
	else if (m_smokeAnimationManager->isPlaying())
	{
		m_smokeAnimationManager->Update(tf);
	}

	m_playerSmoke->setPosition(m_Player->getPosition());

	if (m_Player->IsScreenShaking())
	{
		m_ScreenShaking = true;
		m_CurrentScreenShakeTime = 0.0f;
		m_CurrentScreenShakeTick = 0.0f;
		m_ScreenShakeOffset.x = (((float)rand() - (RAND_MAX / 2)) / (float)RAND_MAX / 2) * m_ScreenShakeSizeX;
		m_ScreenShakeOffset.y = (((float)rand() - (RAND_MAX / 2)) / (float)RAND_MAX / 2) * m_ScreenShakeSizeY;
	}

	if (m_ScreenShaking)
	{
		m_CurrentScreenShakeTime += tf;
		m_CurrentScreenShakeTick += tf;
		if (m_CurrentScreenShakeTime > m_TotalScreenShakeTime)
		{
			m_ScreenShaking = false;
			m_ScreenShakeOffset = sf::Vector2f(0, 0);
		}
		else
		{
			if (m_CurrentScreenShakeTick > m_ScreenShakeTick)
			{
				//srand(time(NULL));
				m_CurrentScreenShakeTick = 0.0f;
				m_ScreenShakeOffset.x = (((float)rand() / (float)RAND_MAX)) * m_ScreenShakeSizeX * 2 - m_ScreenShakeSizeX;
				m_ScreenShakeOffset.y = (((float)rand() / (float)RAND_MAX)) * m_ScreenShakeSizeY * 2 - m_ScreenShakeSizeY;

				//cout << "X: " << m_ScreenShakeOffset.x << endl;
				//cout << "Y: " << m_ScreenShakeOffset.y << endl;
			}
		}
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
		target.draw(*m_playerSmoke);
	}
	target.draw(*m_UpperBorder.get());
	target.draw(*m_LowerBorder.get());
	target.draw(*m_RightBorder.get());
	target.draw(*m_LeftBorder.get());
}

void SceneManager::loadTitle()
{
	m_CurrentScene = TitleScreen;
}

void SceneManager::loadSpace()
{
	m_CurrentScene = Space;
	m_Scene.reset();
	m_EnemyManager->Clear();
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 10 * 960);

	m_Scene->addBackground(-0.999, "Textures/space/space_stars_small.png");

	m_Scene->addBackground(-0.99, "Textures/space/space_stars_big.png");

	m_Scene->addBackground(-0.98, "Textures/space/space_background_objects.png");

	m_Scene->addBackground(-0.97, "Textures/space/earth.png", -1, false);

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_EnemyManager->generateEnemies(b_Squid, m_viewSizeY / 10, m_viewSizeY / 10, 9);

	m_EnemyManager->generateEnemies(b_Asteroid, m_viewSizeY / 10, m_viewSizeY / 10, 9);

	m_Player->setPosition(640, 200);
	m_Player->SetShellColor(sf::Color(0xf6edcdff));
	m_Player->SetShellGravity(0);
	m_Player->SetFallingParams(500, 100);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));
}

void SceneManager::loadSky()
{
	m_CurrentScene = Sky;
	m_Scene.reset();
	m_EnemyManager->Clear();
	
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 20 * 960);

	//m_Scene->addBackground(-0.999, "Textures/space_stars_small.png");

	m_Scene->addBackground(-0.98, "Textures/sky/sky_backgound_sky.png", -1, false);

	m_Scene->addBackground(-0.99, "Textures/sky/sky_backgound_cloud.png");

	//m_Scene->addBackground(-0.98, "Textures/sky/sky_backgound_ground.png", -1, false);

	m_Scene->addBackground(-0.97, "Textures/sky/sky_backgound_trees.png", -1, false, 300.0f);

	//m_EnemyManager->generateEnemies(b_Bird, m_viewSizeY / 10, m_viewSizeY / 10, 19);

	m_EnemyManager->generateEnemies(b_Cloud, m_viewSizeY / 10, m_viewSizeY / 10, 19);

	m_Scene->setBackgroundFillColor(0x655057ff);
	//m_Scene.reset();
	m_Player->setPosition(640, 200);
	m_Player->SetFallingParams(500, 200);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));
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
	m_UpperBorder->setSize(sf::Vector2f(m_View->getSize().x + m_ScreenShakeSizeX * 2, ((m_View->getSize().y - m_viewSizeY) / 2) + m_ScreenShakeSizeY));
	m_LowerBorder->setSize(sf::Vector2f(m_View->getSize().x + m_ScreenShakeSizeX * 2, ((m_View->getSize().y - m_viewSizeY) / 2) + m_ScreenShakeSizeY));
	m_LeftBorder->setSize(sf::Vector2f((m_View->getSize().x - m_viewSizeX) / 2 + m_ScreenShakeSizeX, m_View->getSize().y + m_ScreenShakeSizeY));
	m_RightBorder->setSize(sf::Vector2f((m_View->getSize().x - m_viewSizeX) / 2 + m_ScreenShakeSizeX, m_View->getSize().y + m_ScreenShakeSizeY));
}

void SceneManager::setWindow(sf::RenderWindow* window)
{
	m_renderWindow = window;
	m_Player->SetWindow(window);
}
