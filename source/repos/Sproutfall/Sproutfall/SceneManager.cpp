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
	m_EnemyManager = make_unique<EnemyManager>(sf::Vector2f(m_viewSizeX, m_viewSizeY), m_Player.get());
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
	else if (scene == Forest)
	{
		loadForest();
	}
	else if (scene == Win)
	{
		loadWin();
	}
}

void SceneManager::Update(float tf)
{
	if (m_LoadSpace)
	{
		m_LoadSpace = false;
		loadSpace();
	}

	if (m_CurrentScene == TitleScreen)
	{
		
	}
	else
	{
		if (m_Player->getPosition().y >= (m_Scene->getLevelSize()))
		{
			loadScene(m_CurrentScene + 1);
		}

		if (m_Player->getStatus())
		{
			m_Player->Update(tf);
			m_EnemyManager->Update(tf);
			m_Scene->Update(tf);
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
}

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Vector2f playerPos;

	if (m_Scene->hasGameplay())
	{
		if (m_Player->getPosition().y < (m_Scene->getLevelSize()) - (m_viewSizeY / 2))
		{
			playerPos = m_Player->getPosition();
		}
		else
		{
			playerPos = sf::Vector2f(m_viewSizeX / 2, (m_Scene->getLevelSize()) - (m_viewSizeY / 2));
		}
	}
	else
	{
		playerPos = sf::Vector2f(m_viewSizeX / 2, m_viewSizeY / 2);
	}


	m_View->setCenter(m_viewSizeX / 2 + m_ScreenShakeOffset.x, playerPos.y + m_ScreenShakeOffset.y);
	m_UpperBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2) - m_ScreenShakeSizeY);
	m_LeftBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2) - m_ScreenShakeSizeY);
	m_LowerBorder->setPosition((m_viewSizeX / 2) - (m_View->getSize().x / 2) - m_ScreenShakeSizeX, playerPos.y + (m_View->getSize().y / 2) - m_LowerBorder->getSize().y + m_ScreenShakeSizeY);
	m_RightBorder->setPosition((m_viewSizeX / 2) + (m_View->getSize().x / 2) - m_RightBorder->getSize().x + m_ScreenShakeSizeX, playerPos.y - (m_View->getSize().y / 2));
	m_Scene->UpdateBackgroundPositions(playerPos.y);

	if (m_Player->getPosition().y < (m_Scene->getLevelSize() * m_viewSizeY) - (m_viewSizeY / 2))
	{
		if (!m_Scene->getParallax())
		{
			m_Scene->setParallax(true);
		}
		target.setView(*m_View);
	}

	if (!m_Scene->hasGameplay())
	{
		target.setView(*m_View);
	}

	target.draw(*m_Scene);

	if (m_Scene->hasGameplay())
	{
		target.draw(*m_EnemyManager);
		if (m_Player->getStatus())
		{
			target.draw(*m_Player);
		}
		else if (m_smokeAnimationManager->isPlaying())
		{
			target.draw(*m_playerSmoke);
		}
	}

	target.draw(*m_UpperBorder.get());
	target.draw(*m_LowerBorder.get());
	target.draw(*m_RightBorder.get());
	target.draw(*m_LeftBorder.get());

	for (int i = 0; i < m_UILayers.size(); i++)
	{
		target.draw(*m_UILayers[i]);
	}
}

void SceneManager::loadTitle()
{
	m_CurrentScene = TitleScreen;

	m_Scene->setGameplay(false);

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_UILayers.clear();

	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->setCurrent(true);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/PlayButton.png", &m_LoadSpace, sf::IntRect(0,0,140,50), sf::IntRect(140, 0, 140, 50), sf::IntRect(0, 0, 140, 50));

	m_UILayers[m_UILayers.size() - 1]->SetPosition(sf::Vector2f(m_viewSizeX / 2 - 140, m_viewSizeY / 2));

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/logov2.png", sf::Vector2f(m_viewSizeX / 2 - 480, 100));

}

void SceneManager::loadSpace()
{
	m_CurrentScene = Space;
	m_Scene.reset();

	m_UILayers.clear();

	m_EnemyManager->Clear();
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 9600);

	m_Scene->addBackground(0.0, 8920, "Textures/space/space_stars_small.png", 9120);

	m_Scene->addBackground(0.0, 8720, "Textures/space/space_stars_big.png", 9120);

	//m_Scene->addBackground(0.0, 8520, "Textures/space/space_background_objects.png", 9120);

	

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 65, 41));
	m_Scene->addBackgroundElement(sf::Vector2f(860, 0), 8620, "Textures/space/space_saturn.png", frameVector, 1.0f, 9120, -250);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 28, 26));
	m_Scene->addBackgroundElement(sf::Vector2f(250, 0), 8620, "Textures/space/space_pluto.png", frameVector, 1.0f, 9120, 100);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 54, 48));
	m_Scene->addBackgroundElement(sf::Vector2f(550, 0), 8620, "Textures/space/space_moon.png", frameVector, 1.0f, 9120, 400);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 46, 27));
	m_Scene->addBackgroundElement(sf::Vector2f(1050, 0), 8620, "Textures/space/space_satellite.png", frameVector, 1.0f, 9120, 350);
	frameVector.clear();


	for (int i = 0; i < 6; i++)
	{
		frameVector.push_back(sf::IntRect(i * 120, 0, 120, 120));
	}

	for (int i = 0; i < 20; i++)
	{
		frameVector.push_back(sf::IntRect(0, 0, 0, 0));
	}
	m_Scene->addBackgroundElement(sf::Vector2f(105, 0), 8620, "Textures/space/space_meteor.png", frameVector, 0.2f, 9120, 450);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 640, 252));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 4000), 50000, "Textures/space/earth2.png", frameVector, 1.0f, 50000, 360);
	frameVector.clear();

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_EnemyManager->generateEnemies(b_Squid, m_viewSizeY / 10, m_viewSizeY / 10, 9);

	m_EnemyManager->generateEnemies(b_Asteroid, m_viewSizeY / 10, m_viewSizeY / 10, 9);

	m_Player->setPosition(640, 200);
	m_Player->SetShellColor(sf::Color(0xf6edcdff));
	m_Player->SetShellGravity(0);
	m_Player->SetFallingParams(500, 100);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));

	m_Scene->setGameplay(true);
}

void SceneManager::loadSky()
{
	m_CurrentScene = Sky;
	m_Scene.reset();
	m_EnemyManager->Clear();
	
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 20 * 960); //19200

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 640, 600));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 18600, "Textures/sky/sky_backgound_sky.png", frameVector, 1.0f, 19200, 0);
	frameVector.clear();

	m_Scene->addBackground(0.0, 18400, "Textures/sky/sky_backgound_cloud_small.png", 19200);

	frameVector.push_back(sf::IntRect(144, 24, 6, 14));
	m_Scene->addBackgroundElement(sf::Vector2f(300, 0), 18300, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 900);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(16, 11, 26, 42));
	m_Scene->addBackgroundElement(sf::Vector2f(550, 0), 18300, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 500);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(72, 25, 37, 17));
	m_Scene->addBackgroundElement(sf::Vector2f(1100, 0), 18250, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 1000);
	frameVector.clear();

	m_Scene->addBackground(0.0, 18200, "Textures/sky/sky_backgound_cloud_mid.png", 19200);

	//Bird animation config
	for (int i = 0; i < 8; i++)
	{
		frameVector.push_back(sf::IntRect(i * 46, 0, 46, 32));
	}

	m_Scene->addBackgroundElement(sf::Vector2f(100, 0), 18000, "Textures/sky/birds_46.png", frameVector, 0.25f, 19200, 1100);
	frameVector.clear();

	m_Scene->addBackground(0.0, 17200, "Textures/sky/sky_backgound_cloud.png", 19200);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	//m_Scene->addBackgroundElement(sf::Vector2f(0, 300), -0.97, "Textures/sky/sky_backgound_trees.png", frameVector, 1.0f, true, 19200.0f - 480.0f);
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 16800, "Textures/sky/sky_backgound_trees.png", frameVector, 1.0f, 19200, 1860);
	frameVector.clear();

	m_EnemyManager->generateEnemies(b_Bird, m_viewSizeY / 5, m_viewSizeY / 5, 19);

	m_EnemyManager->generateEnemies(b_Cloud, m_viewSizeY / 10, m_viewSizeY / 10, 19);

	m_Scene->setBackgroundFillColor(0x655057ff);
	//m_Scene.reset();
	m_Player->setPosition(640, 200);
	m_Player->SetFallingParams(500, 300);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));

	m_Scene->setGameplay(true);
}

void SceneManager::loadForest()
{
	m_CurrentScene = Forest;
	m_Scene.reset();
	m_EnemyManager->Clear();

	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 30 * 960); //28800
	vector<sf::IntRect> frameVector;

	float fogOffset = 1000;

	m_Scene->addBackground(0.0, 28340, "Textures/forest/forest_level_1.png", 28800);

	m_Scene->addBackground(0.0, 28140, "Textures/forest/forest_level_2.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_3.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_Scene->addBackground(0.0, 27940, "Textures/forest/forest_level_4.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_5.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_Scene->addBackground(0.0, 27740, "Textures/forest/forest_level_6.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_7.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_Scene->setBackgroundFillColor(0x655057ff);
	m_Player->setPosition(640, 200);
	m_Player->SetFallingParams(500, 400);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));

	m_Scene->setGameplay(true);
}

void SceneManager::loadWin()
{
	m_CurrentScene = Win;
}

void SceneManager::handleInput(sf::Event* event)
{
	if (m_Scene->hasGameplay())
	{
		m_Player->handleInput(event);
	}
	if (event->type == sf::Event::MouseButtonPressed)
	{
		for (int i = 0; i < m_UILayers.size(); i++)
		{
			m_UILayers[i]->checkClick(static_cast<sf::Vector2f>(m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow))));
		}
	}
	else if (event->type == sf::Event::MouseButtonReleased)
	{
		for (int i = 0; i < m_UILayers.size(); i++)
		{
			m_UILayers[i]->checkClickRelease(static_cast<sf::Vector2f>(m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow))));
		}
	}
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
