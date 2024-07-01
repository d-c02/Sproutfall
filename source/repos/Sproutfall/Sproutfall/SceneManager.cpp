#include "SFML_Lib.h"
#include "SceneManager.h"

SceneManager::SceneManager(float windowSizeX, float windowSizeY, sf::RenderWindow* window)
{
	m_renderWindow = window;
	m_Player = make_unique<Player>(window);
	m_Player->setVolume(m_SFXVolumeSlider);
	//m_viewSizeX = viewSizeX;
	//m_viewSizeY = viewSizeY;
	m_View = make_unique<sf::View>();
	//m_View->setSize(m_viewSizeX, m_viewSizeY);
	m_EnemyManager = make_unique<EnemyManager>(sf::Vector2f(m_viewSizeX, m_viewSizeY), m_Player.get());
	m_playerSmoke = make_unique<sf::Sprite>();
	m_smokeTexture = make_unique<sf::Texture>();
	if (!m_smokeTexture->loadFromFile("Textures/playerSmoke.png"))
	{
		cout << "Player smoke texture load failure" << endl;
	}

	m_TransitionTexture = make_unique<sf::Texture>();
	if (!m_TransitionTexture->loadFromFile("Textures/TransitionWipe.png"))
	{
		cout << "Transition wipe texture load failure" << endl;
	}
	m_TransitionSprite = make_unique<sf::Sprite>();
	m_TransitionSprite->setScale(2, 2);
	m_TransitionSprite->setTexture(*m_TransitionTexture);
	m_TransitionSprite->setOrigin(0, m_TransitionTexture->getSize().y / 2);

	m_MenuStartSoundBuffer = make_unique<sf::SoundBuffer>();
	if (!m_MenuStartSoundBuffer->loadFromFile("Sounds/start.ogg"))
	{
		cout << "Menu start sound load failure" << endl;
	}
	m_MenuStartSound = make_unique<sf::Sound>();
	m_MenuStartSound->setBuffer(*m_MenuStartSoundBuffer);


	m_TransitionSoundBuffer = make_unique<sf::SoundBuffer>();
	if (!m_TransitionSoundBuffer->loadFromFile("Sounds/transition.ogg"))
	{
		cout << "Menu start sound load failure" << endl;
	}
	m_TransitionSound = make_unique<sf::Sound>();
	m_TransitionSound->setBuffer(*m_TransitionSoundBuffer);

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

	m_Music = make_unique<sf::Music>();
	m_Music->setLoop(true);
	m_Music->setVolume(m_MusicVolumeSlider);

	m_HighscoreManager = make_unique<HighScoreManager>();
	//m_HighscoreManager->AddScore("NUT", 20, 45);
	//m_HighscoreManager->AddScore("FOX", 12, 33);
	//m_HighscoreManager->AddScore("CRE", 20, 58);
	//m_HighscoreManager->AddScore("SRF", 5, 3);
	//m_HighscoreManager->AddScore("BDN", 2, 10);

	m_LetterAtlas = make_unique<sf::Texture>();
	if (!m_LetterAtlas->loadFromFile("Textures/UI/fontspritesheet.png"))
	{
		cout << "Text atlas loading failure" << endl;
	}

	m_Timer = make_unique<Timer>(m_LetterAtlas.get(), sf::Vector2f(m_View->getCenter().x - 64, m_View->getCenter().y - m_viewSizeY / 2 + 64));

	configureUI();
}
SceneManager::~SceneManager()
{

}
void SceneManager::loadScene(int scene)
{
	m_Player->resetGameplay();
	if (scene != Win)
	{
		m_SpeedUpCredits = false;
	}
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
	m_Music->play();
}

void SceneManager::loadSceneWithTransition(int scene)
{
	m_Transitioning = true;
	m_TransitionFlipped = false;
	//m_Music->stop();
	m_TransitionSprite->setPosition(m_TransitionSprite->getPosition().x, m_View->getCenter().y + m_viewSizeY / 2 + m_TransitionTexture->getSize().y);
	m_TransitionSprite->setScale(2, 2);
	m_nextScene = scene;
	if (m_CurrentScene != TitleScreen)
	{
		m_TransitionSound->play();
	}
}

void SceneManager::Update(float tf)
{
	if (!m_Transitioning)
	{
		handleUIInput();
	}
	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->Update(tf);
	}
	//Game Logic
	if (m_Scene->hasGameplay() && !m_Paused && !m_Transitioning)
	{
		if (m_Player->getPosition().y >= (m_Scene->getLevelSize()))
		{
			loadSceneWithTransition(m_CurrentScene + 1);
		}

		if (m_Player->getStatus())
		{
			m_Player->Update(tf);
			m_EnemyManager->Update(tf);
			m_Scene->Update(tf);
			m_Timer->Update(tf);
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
			m_TotalScreenShakeTime = m_ShootScreenShakeTime;
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

	else if (m_CurrentScene == Win && !m_Transitioning)
	{
		if (m_SpeedUpCredits)
		{
			tf *= m_CreditsSpeedUpFactor;
		}
		m_Scene->Update(tf);

		if (m_Scene->isScreenShaking())
		{
			m_ScreenShaking = true;
			m_TotalScreenShakeTime = m_CutsceneScreenShakeTime;
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
	if (m_Transitioning)
	{
		if (m_StartSoundPlayed && m_MenuStartSound->getStatus() == sf::Sound::Status::Stopped)
		{
			if (!m_TransitionFlipped && m_TransitionSprite->getPosition().y > m_View->getCenter().y - m_viewSizeY / 2 - 25)
			{
				m_TransitionSprite->move(0, m_TransitionSpeed * tf);
			}
			if (!m_TransitionFlipped && m_TransitionSprite->getPosition().y < m_View->getCenter().y - m_viewSizeY / 2 - 50 + m_TransitionTexture->getSize().y)
			{
				loadScene(m_nextScene);
				m_Music->play();
				m_TransitionFlipped = true;
				m_Player->Update(0.00001f);
				m_Scene->Update(0.00001f);
				m_EnemyManager->Update(0.00001f);
				m_View->setCenter(m_viewSizeX / 2, m_Player->getPosition().y);
				m_TransitionSprite->setPosition(m_TransitionSprite->getPosition().x, m_View->getCenter().y - m_viewSizeY / 2 + m_TransitionTexture->getSize().y);
				m_TransitionSprite->setScale(2, -2);
			}
			if (m_TransitionFlipped && m_TransitionSprite->getPosition().y > m_View->getCenter().y - m_viewSizeY);
			{
				m_TransitionSprite->move(0, m_TransitionSpeed * tf);
			}
			if (m_TransitionFlipped && m_TransitionSprite->getPosition().y < m_View->getCenter().y - m_viewSizeY - 25)
			{
				m_Transitioning = false;
				if (m_Scene->hasGameplay())
				{
					//m_UILayers[UI_Gameplay_HUD]->setCurrent(true);
				}
			}
		}
		else if (!m_StartSoundPlayed && m_MenuStartSound->getStatus() == sf::Sound::Status::Stopped)
		{
			m_StartSoundPlayed = true;
			m_TransitionSound->play();
		}
	}
}

void SceneManager::handleUIInput()
{
	//Title screen
	if (m_LoadSpace)
	{
		m_LoadSpace = false;
		if (m_CurrentScene == TitleScreen)
		{
			m_MenuStartSound->play();
			m_StartSoundPlayed = false;
		}
		loadSceneWithTransition(Space);
	}
	
	if (m_LoadSky)
	{
		m_LoadSky = false;
		loadSceneWithTransition(Sky);
	}

	if (m_LoadForest)
	{
		m_LoadForest = false;
		loadSceneWithTransition(Forest);
	}

	if (m_Retry)
	{
		m_Paused = false;
		m_Retry = false;
		m_Player->resetGameplay();
		loadScene(m_CurrentScene);
		m_smokeAnimationManager->setState(0);
		if (m_CurrentScene == Space)
		{
			m_Timer->Reset();
		}
	}

	if (m_QuitGame)
	{
		m_QuitGame = false;
		m_renderWindow->close();
	}

	if (m_CloseOptions)
	{
		m_CloseOptions = false;
		for (int i = 0; i < m_UILayers.size(); i++)
		{
			m_UILayers[i]->setCurrent(false);
		}

		if (m_CurrentScene == TitleScreen)
		{
			m_UILayers[UI_Title_Main]->setCurrent(true);
		}
		else if (m_Scene->hasGameplay())
		{
			m_UILayers[UI_Gameplay_HUD]->setCurrent(true);
			m_UILayers[UI_Gameplay_Paused]->setCurrent(true);
		}
		
	}

	if (m_OpenOptions)
	{
		m_OpenOptions = false;
		for (int i = 0; i < m_UILayers.size(); i++)
		{
			m_UILayers[i]->setCurrent(false);
		}
		m_UILayers[UI_Title_Options]->setCurrent(true);
	}

	if (m_SFXVolumeSliderHeld)
	{
		m_Player->playDemoSound();
		m_Player->setVolume(m_SFXVolumeSlider);
		float volume = m_SFXVolumeSlider;
		if (volume < 1)
		{
			volume = 0;
		}
		m_MenuStartSound->setVolume(volume);
		m_TransitionSound->setVolume(volume);
	}

	if (m_MusicVolumeSliderHeld)
	{
		m_Music->setVolume(m_MusicVolumeSlider);
	}

	if (m_LoadTitle)
	{
		m_LoadTitle = false;
		m_Paused = false;
		m_Player->resetGameplay();
		m_smokeAnimationManager->setState(0);
		m_Timer->Reset();
		loadTitle();
	}

	if (m_ClosePauseMenu)
	{
		m_ClosePauseMenu = false;
		m_Paused = false;
		m_UILayers[UI_Gameplay_Paused]->setCurrent(false);
	}
}

void SceneManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//cout << m_View->getCenter().x << " " << m_View->getCenter().y << endl;
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
	else if (m_CurrentScene == Win)
	{
		playerPos = m_Scene->getViewCenter();
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

	m_Timer->setPosition(sf::Vector2f(m_View->getCenter().x - 64, m_View->getCenter().y - m_viewSizeY / 2 + 64));
	//m_UILayers[UI_Gameplay_HUD]->SetPosition(sf::Vector2f(m_View->getCenter().x + m_viewSizeX / 2 - (m_Player->getMaxBullets() * 64), playerPos.y + m_viewSizeY / 2 - 64));

	m_UILayers[UI_Gameplay_Paused]->SetPosition(sf::Vector2f(m_View->getCenter().x - m_viewSizeX / 2, m_View->getCenter().y - m_viewSizeY / 2));
	m_UILayers[UI_Gameplay_HUD]->SetPosition(sf::Vector2f(m_View->getCenter().x - m_viewSizeX / 2, m_View->getCenter().y - m_viewSizeY / 2));
	//m_UILayers[UI_Gameplay_HUD]->SetPosition(sf::Vector2f(m_View->getCenter().x, m_View->getCenter().y));
	m_UILayers[UI_Title_Options]->SetPosition(sf::Vector2f(m_View->getCenter().x - m_viewSizeX / 2, m_View->getCenter().y - m_viewSizeY / 2));

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

	if (m_Scene->hasGameplay())
	{
		target.draw(*m_Timer);
	}

	for (int i = 0; i < m_UILayers.size(); i++)
	{
		target.draw(*m_UILayers[i]);
	}

	if (m_Transitioning)
	{
		target.draw(*m_TransitionSprite);
	}
}

void SceneManager::loadTitle()
{

	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->setCurrent(false);
		m_UILayers[i]->SetPosition(sf::Vector2f(0, 0));
	}

	m_UILayers[UI_Title_Main]->setCurrent(true);

	m_UILayers[UI_Highscore_Input]->setCurrent(true);

	m_CurrentScene = TitleScreen;

	m_Scene.reset();

	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY);

	m_Scene->setGameplay(false);

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_Scene->addBackground(0.0, 8920, "Textures/space/space_stars_small.png", 9120);

	m_Scene->addBackground(0.0, 8720, "Textures/space/space_stars_big.png", 9120);

	vector<sf::IntRect> frameVector;
	frameVector.push_back(sf::IntRect(0, 0, 640, 252));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 0, "Textures/space/earth2.png", frameVector, 1.0f, 10, 500);
	frameVector.clear();

	m_Music->stop();
	if (!m_Music->openFromFile("Sounds/Music/jame_gam_main_theme.ogg"))
	{
		cout << "Title music load failure" << endl;
	}
	m_Music->play();
}

void SceneManager::loadSpace()
{
	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->setCurrent(false);
		m_UILayers[i]->SetPosition(sf::Vector2f(0, 0));
	}

	m_UILayers[UI_Gameplay_HUD]->setCurrent(true);

	m_CurrentScene = Space;
	m_Scene.reset();

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
	m_Scene->addBackgroundElement(sf::Vector2f(0, 4000), 50000, "Textures/space/earth2.png", frameVector, 1.0f, 50000, 390);
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

	//m_Music->stop();
	if (!m_Music->openFromFile("Sounds/Music/jame_gam_space_v2.ogg"))
	{
		cout << "Space music load failure" << endl;
	}

	//m_Music->play();
}

void SceneManager::loadSky()
{
	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->setCurrent(false);
		m_UILayers[i]->SetPosition(sf::Vector2f(0, 0));
	}

	m_UILayers[UI_Gameplay_HUD]->setCurrent(true);

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

	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 16800, "Textures/sky/sky_backgound_trees.png", frameVector, 1.0f, 19200, 1860);
	frameVector.clear();

	//m_EnemyManager->generateEnemies(b_Bird, m_viewSizeY / 5, m_viewSizeY / 5, 19);

	//m_EnemyManager->generateEnemies(b_Cloud, m_viewSizeY / 10, m_viewSizeY / 10, 19);

	m_Scene->setBackgroundFillColor(0x655057ff);

	m_Player->setPosition(640, 200);
	m_Player->SetFallingParams(500, 300);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));

	m_Scene->setGameplay(true);

	//m_Music->stop();
	if (!m_Music->openFromFile("Sounds/Music/jame_gam_sky.ogg"))
	{
		cout << "Sky music load failure" << endl;
	}
	//m_Music->play();
}

void SceneManager::loadForest()
{
	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->setCurrent(false);
		m_UILayers[i]->SetPosition(sf::Vector2f(0, 0));
	}

	m_UILayers[UI_Gameplay_HUD]->setCurrent(true);

	m_CurrentScene = Forest;
	m_Scene.reset();
	m_EnemyManager->Clear();

	//m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 30 * 960); //28800
	m_Scene = make_unique<Scene>(m_Player.get(), m_viewSizeX, m_viewSizeY, 1 * 960);
	vector<sf::IntRect> frameVector;

	m_Scene->addBackground(0.0, 18000, "Textures/forest/forest_level_1.png", 28800);

	m_Scene->addBackground(0.0, 17600, "Textures/forest/forest_level_2.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_3.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_Scene->addBackground(0.0, 17200, "Textures/forest/forest_level_4.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_5.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_Scene->addBackground(0.0, 16800, "Textures/forest/forest_level_6.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	m_Scene->addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_7.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	//m_EnemyManager->generateEnemies(b_Squirrel, m_viewSizeY / 8, m_viewSizeY / 8, 29);

	//m_EnemyManager->generateEnemies(b_Pinecone, m_viewSizeY / 8, m_viewSizeY / 8, 29);

	m_Scene->setBackgroundFillColor(0x655057ff);
	m_Player->setPosition(640, 200);
	m_Player->SetFallingParams(500, 400);

	m_Player->SetOutlineColor(sf::Glsl::Vec4(0.396078431372549f, 0.3137254901960784f, 0.3411764705882353f, 1.0f));

	m_Scene->setGameplay(true);

	//m_Music->stop();
	if (!m_Music->openFromFile("Sounds/Music/jame_gam_29_v4.ogg"))
	{
		cout << "Forest music load failure" << endl;
	}
	//m_Music->play();
}

void SceneManager::loadWin()
{
	for (int i = 0; i < m_UILayers.size(); i++)
	{
		m_UILayers[i]->setCurrent(false);
		m_UILayers[i]->SetPosition(sf::Vector2f(0, 0));
	}

	m_UILayers[UI_Gameplay_HUD]->setCurrent(false);

	m_CurrentScene = Win;
	m_Scene.reset();
	m_EnemyManager->Clear();

	m_Scene = make_unique<FinalCutscene>(m_viewSizeX, m_viewSizeY, m_TransitionTexture.get());

	m_Scene->setGameplay(false);

	//m_Player->setPosition(0, 0);
	m_Player->resetGameplay();
	m_Player->setPosition(640, 370);

	m_Scene->setBackgroundFillColor(0x655057ff);

	if (!m_Music->openFromFile("Sounds/Music/jame_game_song_ending.ogg"))
	{
		cout << "Win music load failure" << endl;
	}

	m_Scene->addBackground(0.0, 0, "Textures/finalcutscene/forest_level_ground_back2.png", m_viewSizeY, false);
}

void SceneManager::handleInput(sf::Event* event)
{
	if (!m_Transitioning)
	{
		if (m_Scene->hasGameplay() && !m_Paused)
		{
			m_Player->handleInput(event);
		}
		for (int i = 0; i < m_UILayers.size(); i++)
		{
			m_UILayers[i]->handleInput(event);
		}
		if (m_Scene->hasGameplay())
		{
			if (event->type == sf::Event::KeyPressed)
			{
				if (event->key.code == sf::Keyboard::Escape)
				{
					if (m_UILayers[UI_Gameplay_Paused]->isCurrent())
					{
						m_Paused = false;
						m_ClosePauseMenu = true;
					}
					else if (m_UILayers[UI_Title_Options]->isCurrent())
					{
						m_CloseOptions = true;
					}
					else
					{
						m_UILayers[UI_Gameplay_Paused]->setCurrent(true);
						m_Paused = true;
					}
				}
			}
		}
		if (m_CurrentScene == Win)
		{
			if (event->type == sf::Event::MouseButtonPressed || event->type == sf::Event::KeyPressed)
			{
				if (event->key.code == sf::Keyboard::Space || event->mouseButton.button == sf::Mouse::Left)
				{
					m_SpeedUpCredits = true;
				}
			}
			else if (event->type == sf::Event::MouseButtonReleased || event->type == sf::Event::KeyReleased)
			{
				if (event->key.code == sf::Keyboard::Space || event->mouseButton.button == sf::Mouse::Left)
				{
					m_SpeedUpCredits = false;
				}
			}
		}
	}
}


void SceneManager::borderView(int width, int height)
{

}

void SceneManager::configureUI()
{
	m_UILayers.clear();

	//Main title
	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/PlayButtonBorder.png", &m_LoadSpace, sf::Vector2f(m_viewSizeX / 2 - 140, m_viewSizeY / 2), sf::IntRect(0, 0, 140, 50), sf::IntRect(140, 0, 140, 50), sf::IntRect(0, 0, 140, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/OptionsButtonOutline2.png", &m_OpenOptions, sf::Vector2f(m_viewSizeX / 2 - 225, m_viewSizeY / 2 + 250), sf::IntRect(0, 0, 225, 50), sf::IntRect(225, 0, 225, 50), sf::IntRect(0, 0, 225, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/QuitButtonOutline.png", &m_QuitGame, sf::Vector2f(m_viewSizeX / 2 - 140, m_viewSizeY / 2 + 350), sf::IntRect(0, 0, 140, 50), sf::IntRect(140, 0, 140, 50), sf::IntRect(0, 0, 140, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/debug_skyButton.png", &m_LoadSky, sf::Vector2f(1000, 400), sf::IntRect(0, 0, 100, 50), sf::IntRect(0, 0, 100, 50), sf::IntRect(0, 0, 100, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/debug_woodsButton.png", &m_LoadForest, sf::Vector2f(1000, 600), sf::IntRect(0, 0, 100, 50), sf::IntRect(0, 0, 100, 50), sf::IntRect(0, 0, 100, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/logov2.png", sf::Vector2f(m_viewSizeX / 2 - 480, 50));

	//Main title options
	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/MenuBorderSmallRough.png", sf::Vector2f(100, 75));

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/CloseButtonRough.png", &m_CloseOptions, sf::Vector2f(1040, 125), sf::IntRect(0, 0, 50, 50), sf::IntRect(50, 0, 50, 50), sf::IntRect(0, 0, 50, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/SFXIconRough.png", sf::Vector2f(175, 250));

	m_UILayers[m_UILayers.size() - 1]->AddSlider(sf::Vector2f(275, 250), &m_SFXVolumeSlider, &m_SFXVolumeSliderHeld, "Textures/UI/SliderBarRough.png", "Textures/UI/SliderNubRough.png", 0, 100.0f, m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/MusicIcon.png", sf::Vector2f(150, 400));

	m_UILayers[m_UILayers.size() - 1]->AddSlider(sf::Vector2f(275, 400), &m_MusicVolumeSlider, &m_MusicVolumeSliderHeld, "Textures/UI/SliderBarRough.png", "Textures/UI/SliderNubRough.png", 0, 100.0f, m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/FullscreenIcon.png", sf::Vector2f(150, 550));

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/FullscreenButtonRough.png", &m_Fullscreen, sf::Vector2f(500, 550), sf::IntRect(100, 0, 50, 50), sf::IntRect(50, 0, 50, 50), sf::IntRect(0, 0, 50, 50), m_renderWindow);

	//Gameplay HUD
	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->AddCounter("Textures/UI/UIBulletSheet.png", m_Player->getBulletPointer(), m_Player->getMaxBullets(), sf::IntRect(0, 0, 64, 64), sf::IntRect(64, 0, 64, 64), sf::Vector2f(m_View->getCenter().x + m_viewSizeX / 2 - (m_Player->getMaxBullets() * 32), m_View->getCenter().y + m_viewSizeY / 2 - 64));

	m_UILayers[m_UILayers.size() - 1]->AddCounter("Textures/UI/UIHealthSheet.png", m_Player->getHealthPointer(), m_Player->getMaxHealth(), sf::IntRect(0, 0, 64, 64), sf::IntRect(64, 0, 64, 64), sf::Vector2f(m_View->getCenter().x - m_viewSizeX / 2 + (m_Player->getMaxHealth() * 64), m_View->getCenter().y - m_viewSizeY / 2 + 32));

	//Gameplay paused
	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->AddVisualElement("Textures/UI/MenuBorderSmallRough.png", sf::Vector2f(100, 75));

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/ResumeButton.png", &m_ClosePauseMenu, sf::Vector2f(m_viewSizeX / 2 - 140, 125), sf::IntRect(0, 0, 140, 50), sf::IntRect(140, 0, 140, 50), sf::IntRect(0, 0, 140, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/OptionsButton2.png", &m_OpenOptions, sf::Vector2f(m_viewSizeX / 2 - 140, 275), sf::IntRect(0, 0, 140, 50), sf::IntRect(140, 0, 140, 50), sf::IntRect(0, 0, 140, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/RetryButton.png", &m_Retry, sf::Vector2f(m_viewSizeX / 2 - 95, 425), sf::IntRect(0, 0, 95, 50), sf::IntRect(95, 0, 95, 50), sf::IntRect(0, 0, 95, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/QuitToTitleButton.png", &m_LoadTitle, sf::Vector2f(m_viewSizeX / 2 - 200, 575), sf::IntRect(0, 0, 200, 50), sf::IntRect(200, 0, 200, 50), sf::IntRect(0, 0, 200, 50), m_renderWindow);

	m_UILayers[m_UILayers.size() - 1]->AddButton("Textures/UI/QuitToDesktopButton.png", &m_QuitGame, sf::Vector2f(m_viewSizeX / 2 - 215, 725), sf::IntRect(0, 0, 215, 50), sf::IntRect(215, 0, 215, 50), sf::IntRect(0, 0, 215, 50), m_renderWindow);

	//Highscore input
	m_UILayers.push_back(make_unique<UIElementLayer>());

	m_UILayers[m_UILayers.size() - 1]->AddStringInput(m_LetterAtlas.get(), m_UsernameInput, 3, sf::Vector2f(m_viewSizeX / 2, m_viewSizeY / 2));
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

bool SceneManager::isFullscreen()
{
	return m_Fullscreen;
}
