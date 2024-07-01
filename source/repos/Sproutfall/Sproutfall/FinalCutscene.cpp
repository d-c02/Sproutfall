#include "FinalCutscene.h"

FinalCutscene::FinalCutscene(float viewSizeX, float viewSizeY, sf::Texture* TransitionTexture)
{
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_playerTexture = make_unique<sf::Texture>();
	if (!m_playerTexture->loadFromFile("Textures/finalcutscene/PlayerSpriteSheet.png"))
	{
		cout << "Final cutscene player texture load failure" << endl;
	}

	m_playerSprite = make_unique<sf::Sprite>();
	m_playerSprite->setTexture(*m_playerTexture);
	m_playerSprite->setOrigin(4, 9);
	m_playerSprite->setScale(2, 2);

	m_playerAnimationManager = make_unique<AnimationManager>(m_playerSprite.get());
	vector<sf::IntRect> frameVector;
	frameVector.push_back(sf::IntRect(0, 0, 8, 14));
	frameVector.push_back(sf::IntRect(8, 0, 8, 14));
	m_playerAnimationManager->addState(0, frameVector, true, 0.2f);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(16, 0, 8, 14));
	frameVector.push_back(sf::IntRect(24, 0, 8, 14));
	frameVector.push_back(sf::IntRect(32, 0, 8, 14));
	frameVector.push_back(sf::IntRect(40, 0, 8, 14));
	frameVector.push_back(sf::IntRect(0, 0, 1, 1));
	m_playerAnimationManager->addState(1, frameVector, false, 0.2f);
	frameVector.clear();

	m_playerAnimationManager->setState(0);

	m_SproutTexture = make_unique<sf::Texture>();
	if (!m_SproutTexture->loadFromFile("Textures/finalcutscene/sprout.png"))
	{
		cout << "Final cutscene sprout texture load failure" << endl;
	}

	m_SproutSprite = make_unique<sf::Sprite>();
	m_SproutSprite->setTexture(*m_SproutTexture);
	m_SproutSprite->setOrigin(75, 480);
	m_SproutSprite->setScale(2, 2);
	m_SproutSprite->setPosition(sf::Vector2f(m_finalPlayerPosition.x, m_finalPlayerPosition.y + 15));

	m_SproutAnimationManager = make_unique<AnimationManager>(m_SproutSprite.get());
	for (int i = 0; i < 27; i++)
	{
		frameVector.push_back(sf::IntRect(i * 150, 0, 150, 480));
	}
	m_SproutAnimationManager->addState(0, frameVector, false, 0.05f);
	m_SproutAnimationManager->setState(0);
	m_SproutSprite->setTextureRect(sf::IntRect(0, 0, 150, 300));
	frameVector.clear();

	m_TransitionTexture = TransitionTexture;
	m_TransitionSprite = make_unique<sf::Sprite>();
	m_TransitionSprite->setScale(2, 2);
	m_TransitionSprite->setTexture(*m_TransitionTexture);
	m_TransitionSprite->setOrigin(0, m_TransitionTexture->getSize().y / 2);
	m_TransitionSprite->setPosition(m_initialTransitionPosition);
	m_TransitionSprite->setScale(2, -2);

	m_GrowingSproutTexture = make_unique<sf::Texture>();
	if (!m_GrowingSproutTexture->loadFromFile("Textures/finalcutscene/growingsprout.png"))
	{
		cout << "Final cutscene growing sprout texture load failure" << endl;
	}

	m_GrowingSproutSprite = make_unique<sf::Sprite>();
	m_GrowingSproutSprite->setTexture(*m_GrowingSproutTexture);
	m_GrowingSproutSprite->setOrigin(75, 0);
	m_GrowingSproutSprite->setScale(2, 2);
	m_GrowingSproutSprite->setPosition(sf::Vector2f(1280 / 2, 960 - 100));

	m_GrowingSproutAnimationManager = make_unique<AnimationManager>(m_GrowingSproutSprite.get());
	for (int i = 0; i < 5; i++)
	{
		frameVector.push_back(sf::IntRect(i * 150, 0, 150, 50));
	}
	m_GrowingSproutAnimationManager->addState(0, frameVector, true, 0.5f);
	m_GrowingSproutAnimationManager->setState(0);
	m_GrowingSproutSprite->setTextureRect(sf::IntRect(0, 0, 150, 450));
	frameVector.clear();

	m_State = s_PlayerFalling;

	setBackgroundFillColor(0x655057ff);
}

void FinalCutscene::Update(float tf)
{
	if (m_State == s_PlayerFalling)
	{
		m_TimeAccumulator += tf;
		m_playerSprite->setPosition(m_finalPlayerPosition * (m_TimeAccumulator / m_MaxFallingTime) + m_initialPlayerPosition * (1.0f - (m_TimeAccumulator / m_MaxFallingTime)));
		m_playerAnimationManager->Update(tf);
		if (m_TimeAccumulator > m_MaxFallingTime)
		{
			m_TimeAccumulator = 0;
			m_State = s_ImpactVFX;
			m_playerAnimationManager->setState(1);
			m_playerAnimationManager->Play();
		}
	}
	else if (m_State == s_ImpactVFX)
	{
		m_playerAnimationManager->Update(tf);
		if (!m_playerAnimationManager->isPlaying())
		{
			m_State = s_Shaking;
			m_ScreenShaking = true;
		}
	}
	else if (m_State == s_Shaking)
	{
		m_TimeAccumulator += tf;
		if (m_TimeAccumulator >= m_MaxShakingTime)
		{
			m_State = s_SproutGrowing;
			m_TimeAccumulator = 0;
		}
	}
	else if (m_State == s_SproutGrowing)
	{
		m_SproutAnimationManager->Update(tf);
		if (!m_SproutAnimationManager->isPlaying())
		{
			m_NextScene = s_Forest;
			UpdateTransitionPos();
			m_State = s_Transitioning;
		}
	}
	else if (m_State == s_Transitioning)
	{
		for (int i = 0; i < m_Backgrounds.size(); i++)
		{
			m_Backgrounds[i]->Update(tf);
		}
		m_TimeAccumulator += tf;
		if (!m_TransitionFlipped)
		{
			m_TransitionSprite->setPosition(m_finalTransitionPosition * (m_TimeAccumulator / m_MaxTransitionTime) + m_initialTransitionPosition * (1.0f - (m_TimeAccumulator / m_MaxTransitionTime)));
		}
		else
		{
			m_TransitionSprite->setPosition(m_finalTransitionPosition2 * (m_TimeAccumulator / m_MaxTransitionTime) + m_finalTransitionPosition * (1.0f - (m_TimeAccumulator / m_MaxTransitionTime)));
		}
		if (m_TimeAccumulator > m_MaxTransitionTime)
		{
			m_TimeAccumulator = 0;
			if (!m_TransitionFlipped)
			{
				m_TransitionFlipped = true;
				m_TransitionSprite->setScale(2, 2);
				loadScene(m_NextScene);
			}
			else
			{
				m_TransitionFlipped = false;
				m_TransitionSprite->setScale(2, -2);
				m_State = m_NextScene;
			}
		}
	}
	else if (m_State == s_Forest)
	{
		for (int i = 0; i < m_Backgrounds.size(); i++)
		{
			m_Backgrounds[i]->Update(tf);
		}
		m_GrowingSproutAnimationManager->Update(tf);
		m_EnemyAnimationManager1->Update(tf);
		m_EnemyAnimationManager2->Update(tf);
		m_BGUpdatePos += m_BGUpdateTick * tf;
		if (!m_HoldingTextPos && !m_MovingPastText)
		{
			if (m_EnemySprite1->getPosition().y >= m_BGUpdatePos)
			{
				m_HoldingTextPos = true;
			}
		}
		else
		{
			m_TimeAccumulator += tf;
			if (!m_MovingPastText)
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_HoldingTextPos = false;
					m_MovingPastText = true;
					m_TimeAccumulator = 0.0f;
				}
			}
			else
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_MovingPastText = false;
					m_TimeAccumulator = 0.0f;
					m_State = s_Transitioning;
					m_NextScene = s_Sky;
					UpdateTransitionPos();
					m_TransitionSprite->setPosition(m_initialTransitionPosition.x, m_BGUpdatePos + m_initialTransitionPosition.y);
				}
			}
		}
	}
	else if (m_State == s_Sky)
	{
		for (int i = 0; i < m_Backgrounds.size(); i++)
		{
    			m_Backgrounds[i]->Update(tf);
		}
		m_GrowingSproutAnimationManager->Update(tf);
		m_EnemyAnimationManager1->Update(tf);
		m_EnemyAnimationManager2->Update(tf);
		m_BGUpdatePos += m_BGUpdateTick * tf;
		if (!m_HoldingTextPos && !m_MovingPastText)
		{
			if (m_EnemySprite1->getPosition().y >= m_BGUpdatePos)
			{
				m_HoldingTextPos = true;
			}
		}
		else
		{
			m_TimeAccumulator += tf;
			if (!m_MovingPastText)
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_HoldingTextPos = false;
					m_MovingPastText = true;
					m_TimeAccumulator = 0.0f;
				}
			}
			else
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_MovingPastText = false;
					m_TimeAccumulator = 0.0f;
					m_State = s_Transitioning;
					UpdateTransitionPos();
					m_NextScene = s_Space;
				}
			}
		}
	}
	else if (m_State == s_Space)
	{
		for (int i = 0; i < m_Backgrounds.size(); i++)
		{
			m_Backgrounds[i]->Update(tf);
		}
		m_GrowingSproutAnimationManager->Update(tf);
		m_EnemyAnimationManager1->Update(tf);
		m_EnemyAnimationManager2->Update(tf);
		m_BGUpdatePos += m_BGUpdateTick * tf;
		if (!m_HoldingTextPos && !m_MovingPastText)
		{
			if (m_EnemySprite1->getPosition().y >= m_BGUpdatePos)
			{
				m_HoldingTextPos = true;
			}
		}
		else
		{
			m_TimeAccumulator += tf;
			if (!m_MovingPastText)
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_HoldingTextPos = false;
					m_MovingPastText = true;
					m_TimeAccumulator = 0.0f;
				}
			}
			else
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_MovingPastText = false;
					m_TimeAccumulator = 0.0f;
					m_State = s_Transitioning;
					UpdateTransitionPos();
					m_NextScene = s_FarOut;
				}
			}
		}
	}
	else if (m_State == s_FarOut)
	{
		for (int i = 0; i < m_Backgrounds.size(); i++)
		{
			m_Backgrounds[i]->Update(tf);
		}
		m_GrowingSproutAnimationManager->Update(tf);
		m_EnemyAnimationManager1->Update(tf);
		m_EnemyAnimationManager2->Update(tf);
		m_BGUpdatePos += m_BGUpdateTick * tf;
		if (!m_HoldingTextPos && !m_MovingPastText)
		{
			if (m_EnemySprite1->getPosition().y >= m_BGUpdatePos)
			{
				m_HoldingTextPos = true;
			}
		}
		else
		{
			m_TimeAccumulator += tf;
			if (!m_MovingPastText)
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_HoldingTextPos = false;
					m_MovingPastText = true;
					m_TimeAccumulator = 0.0f;
				}
			}
			else
			{
				if (m_TimeAccumulator >= m_ReadingTime)
				{
					m_MovingPastText = false;
					m_TimeAccumulator = 0.0f;
					m_State = s_Transitioning;
					UpdateTransitionPos();
					m_NextScene = s_End;
				}
			}
		}
	}
	else if (m_State == s_FarOut)
	{

		}
}

bool FinalCutscene::isScreenShaking()
{
	if (m_ScreenShaking)
	{
		m_ScreenShaking = false;
		return true;
	}
	return m_ScreenShaking;
}

sf::Vector2f FinalCutscene::getViewCenter()
{
	return sf::Vector2f(640, m_BGUpdatePos);
}

void FinalCutscene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear(sf::Color(m_BackgroundFillColor));
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		m_Backgrounds[i]->UpdatePosition(m_BGUpdatePos);
		target.draw(*m_Backgrounds[i]);
	}
	for (int i = 0; i < m_BackgroundSprites.size(); i++)
	{
		target.draw(*m_BackgroundSprites[i]);
	}
	if (m_State == s_PlayerFalling || m_State == s_ImpactVFX)
	{
		target.draw(*m_playerSprite);
	}
	if (m_State == s_SproutGrowing || m_State == s_Transitioning)
	{
		if (!m_TransitionFlipped)
		{
			target.draw(*m_SproutSprite);
		}
		else
		{
			target.draw(*m_GrowingSproutSprite);
		}
		if (m_NextScene == s_Sky || m_NextScene == s_Space || m_NextScene == s_FarOut)
		{
			target.draw(*m_GrowingSproutSprite);
		}
		target.draw(*m_TransitionSprite);
	}
	if (m_State == s_Forest || m_State == s_Sky || m_State == s_Space || m_State == s_FarOut)
	{
		m_GrowingSproutSprite->setPosition(m_GrowingSproutSprite->getPosition().x, m_BGUpdatePos + 380);
		if (m_HoldingTextPos)
		{
			m_EnemySprite1->setPosition(m_EnemySprite1->getPosition().x, m_BGUpdatePos);
			m_EnemySprite2->setPosition(m_EnemySprite2->getPosition().x, m_BGUpdatePos);
			m_TextSprite->setPosition(m_TextSprite->getPosition().x, m_BGUpdatePos);
		}
		if (m_State != s_FarOut)
		{
			target.draw(*m_EnemySprite1);
			target.draw(*m_EnemySprite2);
		}
		target.draw(*m_TextSprite);
		target.draw(*m_GrowingSproutSprite);
	}
}

void FinalCutscene::loadScene(int Scene)
{
	if (Scene == s_Forest)
	{
		loadForest();
	}
	if (Scene == s_Sky)
	{
		loadSky();
	}
	if (Scene == s_Space)
	{
		loadSpace();
	}
	if (Scene == s_FarOut)
	{
		loadFarout();
	}
}

void FinalCutscene::loadForest()
{
	m_Backgrounds.clear();

	vector<sf::IntRect> frameVector;

	addBackground(0.0, 18000, "Textures/forest/forest_level_1.png", 28800);

	addBackground(0.0, 17600, "Textures/forest/forest_level_2.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_3.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	addBackground(0.0, 17200, "Textures/forest/forest_level_4.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_5.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	addBackground(0.0, 16800, "Textures/forest/forest_level_6.png", 28800);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));
	addBackgroundElement(sf::Vector2f(0, 0), 28800, "Textures/forest/forest_level_7.png", frameVector, 0.25f, 28800, -480);
	frameVector.clear();

	m_EnemyTexture1 = make_unique<sf::Texture>();
	if (!m_EnemyTexture1->loadFromFile("Textures/Enemies/forest_squirrel.png"))
	{
		cout << "Squirrel texture load failure" << endl;
	}
	
	m_EnemySprite1 = make_unique<sf::Sprite>();
	m_EnemySprite1->setScale(2, 2);
	m_EnemySprite1->setOrigin(32, 32);
	m_EnemySprite1->setTextureRect(sf::IntRect(64, 0, 64, 64));
	m_EnemySprite1->setPosition(200, m_CreditsOffset);
	m_EnemySprite1->setTexture(*m_EnemyTexture1);

	m_EnemyAnimationManager1 = make_unique<AnimationManager>(m_EnemySprite1.get());

	//Climbing state
	frameVector.push_back(sf::IntRect(64, 0, 64, 64));
	frameVector.push_back(sf::IntRect(128, 0, 64, 64));
	frameVector.push_back(sf::IntRect(192, 0, 64, 64));
	frameVector.push_back(sf::IntRect(256, 0, 64, 64));
	frameVector.push_back(sf::IntRect(320, 0, 64, 64));
	m_EnemyAnimationManager1->addState(0, frameVector, true, 0.2f);
	frameVector.clear();
	
	m_EnemyAnimationManager1->setState(0);
	
	m_EnemyTexture2 = make_unique<sf::Texture>();
	if (!m_EnemyTexture2->loadFromFile("Textures/Enemies/pinebomb_64x64.png"))
	{
		cout << "Pinebomb texture load failure credits" << endl;
	}

	m_EnemySprite2 = make_unique<sf::Sprite>();
	m_EnemySprite2->setScale(2, 2);
	m_EnemySprite2->setOrigin(16, 16);
	m_EnemySprite2->setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_EnemySprite2->setPosition(1000, m_CreditsOffset);
	m_EnemySprite2->setTexture(*m_EnemyTexture2);

	m_EnemyAnimationManager2 = make_unique<AnimationManager>(m_EnemySprite2.get());

	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	m_EnemyAnimationManager2->addState(0, frameVector, false, 1.0f);
	frameVector.clear();

	m_EnemyAnimationManager2->setState(0);

	m_TextTexture = make_unique<sf::Texture>();
	if (!m_TextTexture->loadFromFile("Textures/finalcutscene/MusicByAtticFox.png"))
	{
		cout << "Atticfox credits load failure" << endl;
	}
	m_TextSprite = make_unique<sf::Sprite>();
	m_TextSprite->setTexture(*m_TextTexture);
	m_TextSprite->setOrigin(m_TextTexture->getSize().x / 2, m_TextTexture->getSize().y / 2);
	m_TextSprite->setPosition(640, m_CreditsOffset);
}

void FinalCutscene::loadSky()
{

	m_Backgrounds.clear();
	m_BackgroundSprites.clear();

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 640, 600));
	addBackgroundElement(sf::Vector2f(0, m_BGUpdatePos - 18600), m_BGUpdatePos, "Textures/sky/sky_backgound_sky.png", frameVector, 1.0f, 18600, 0);
	frameVector.clear();
	
	addBackground(0.0, 18400, "Textures/sky/sky_backgound_cloud_small.png", 19200);

	frameVector.push_back(sf::IntRect(144, 24, 6, 14));
	addBackgroundElement(sf::Vector2f(300, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 1100);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(16, 11, 26, 42));
	addBackgroundElement(sf::Vector2f(550, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 1500);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(72, 25, 37, 17));
	addBackgroundElement(sf::Vector2f(1100, m_BGUpdatePos - 18250), m_BGUpdatePos, "Textures/sky/objects_58.png", frameVector, 0.25f, 19200, 900);
	frameVector.clear();

	addBackground(m_BGUpdatePos - 18200, m_BGUpdatePos - 960, "Textures/sky/sky_backgound_cloud_mid.png", 19200);
	
	
	//Bird animation config
	for (int i = 0; i < 8; i++)
	{
		frameVector.push_back(sf::IntRect(i * 46, 0, 46, 32));
	}

	addBackgroundElement(sf::Vector2f(100, m_BGUpdatePos - 18000), m_BGUpdatePos, "Textures/sky/birds_46.png", frameVector, 0.25f, 19200, 1100);
	frameVector.clear();

	addBackground(m_BGUpdatePos - 17200 + 960, m_BGUpdatePos, "Textures/sky/sky_backgound_cloud.png", 19200);

	frameVector.push_back(sf::IntRect(0, 0, 640, 480));

	addBackgroundElement(sf::Vector2f(0, m_BGUpdatePos - 16800), m_BGUpdatePos, "Textures/sky/sky_backgound_trees.png", frameVector, 1.0f, 19200, 1760);
	frameVector.clear();
	
	if (!m_EnemyTexture1->loadFromFile("Textures/Enemies/sky_badcloud.png"))
	{
		cout << "Badcloud texture load failure credits" << endl;
	}

	m_EnemySprite1->setOrigin(16, 16);
	m_EnemySprite1->setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_EnemySprite1->setPosition(200, m_BGUpdatePos + m_CreditsOffset);
	m_EnemySprite1->setTexture(*m_EnemyTexture1);

	m_EnemyAnimationManager1 = make_unique<AnimationManager>(m_EnemySprite1.get());

	//Climbing state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	m_EnemyAnimationManager1->addState(0, frameVector, true, 0.75f);
	frameVector.clear();

	m_EnemyAnimationManager1->setState(0);
	m_EnemyAnimationManager1->Play();

	if (!m_EnemyTexture2->loadFromFile("Textures/Enemies/sky_bird.png"))
	{
		cout << "Bird texture load failure" << endl;
	}

	m_EnemySprite2->setOrigin(16, 16);
	m_EnemySprite2->setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_EnemySprite2->setPosition(1000, m_BGUpdatePos + m_CreditsOffset);
	m_EnemySprite2->setTexture(*m_EnemyTexture2);

	m_EnemyAnimationManager2 = make_unique<AnimationManager>(m_EnemySprite2.get());

	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	m_EnemyAnimationManager2->addState(0, frameVector, true, 0.11f);
	frameVector.clear();

	if (!m_TextTexture->loadFromFile("Textures/finalcutscene/MusicAndSFXByCore3.png"))
	{
		cout << "Core credits load failure" << endl;
	}
	m_TextSprite = make_unique<sf::Sprite>();
	m_TextSprite->setTexture(*m_TextTexture);
	m_TextSprite->setOrigin(m_TextTexture->getSize().x / 2, m_TextTexture->getSize().y / 2);
	m_TextSprite->setPosition(640, m_BGUpdatePos + m_CreditsOffset);
}

void FinalCutscene::loadSpace()
{
	m_Backgrounds.clear();
	m_BackgroundSprites.clear();

	addBackground(0.0, 8920, "Textures/space/space_stars_small.png", 9120);

	addBackground(0.0, 8720, "Textures/space/space_stars_big.png", 9120);

	//m_Scene->addBackground(0.0, 8520, "Textures/space/space_background_objects.png", 9120);

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 65, 41));
	addBackgroundElement(sf::Vector2f(860, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/space/space_saturn.png", frameVector, 1.0f, 19200, 250);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 28, 26));
	addBackgroundElement(sf::Vector2f(250, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/space/space_pluto.png", frameVector, 1.0f, 19200, 100);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 54, 48));
	addBackgroundElement(sf::Vector2f(550, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/space/space_moon.png", frameVector, 1.0f, 19200, 600);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 46, 27));
	addBackgroundElement(sf::Vector2f(1050, m_BGUpdatePos - 18250), m_BGUpdatePos, "Textures/space/space_satellite.png", frameVector, 0.25f, 19200, 950);
	frameVector.clear();


	for (int i = 0; i < 6; i++)
	{
		frameVector.push_back(sf::IntRect(i * 120, 0, 120, 120));
	}

	for (int i = 0; i < 20; i++)
	{
		frameVector.push_back(sf::IntRect(0, 0, 0, 0));
	}
	addBackgroundElement(sf::Vector2f(105, m_BGUpdatePos - 18000), m_BGUpdatePos, "Textures/space/space_meteor.png", frameVector, 0.25f, 19200, 550);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 640, 252));
	addBackgroundElement(sf::Vector2f(0, m_BGUpdatePos - 16800), m_BGUpdatePos, "Textures/space/earth2.png", frameVector, 1.0f, 19200, 2100);
	frameVector.clear();

	if (!m_EnemyTexture1->loadFromFile("Textures/Enemies/squid.png"))
	{
		cout << "Squid texture load failure credits" << endl;
	}

	m_EnemySprite1->setOrigin(12, 13);
	m_EnemySprite1->setTextureRect(sf::IntRect(173, 0, 25, 26));
	m_EnemySprite1->setPosition(200, m_BGUpdatePos + m_CreditsOffset);
	m_EnemySprite1->setTexture(*m_EnemyTexture1);

	m_EnemyAnimationManager1 = make_unique<AnimationManager>(m_EnemySprite1.get());

	//Climbing state
	frameVector.push_back(sf::IntRect(173, 0, 25, 26));
	frameVector.push_back(sf::IntRect(199, 0, 26, 26));
	m_EnemyAnimationManager1->addState(0, frameVector, true, 1.0f);
	frameVector.clear();

	m_EnemyAnimationManager1->setState(0);
	m_EnemyAnimationManager1->Play();

	if (!m_EnemyTexture2->loadFromFile("Textures/Enemies/asteroid.png"))
	{
		cout << "Asteroid texture load failure credits" << endl;
	}

	m_EnemySprite2->setOrigin(15, 13);
	m_EnemySprite2->setTextureRect(sf::IntRect(0, 0, 30, 26));
	m_EnemySprite2->setPosition(1000, m_BGUpdatePos + m_CreditsOffset);
	m_EnemySprite2->setTexture(*m_EnemyTexture2);

	m_EnemyAnimationManager2 = make_unique<AnimationManager>(m_EnemySprite2.get());

	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 30, 26));
	m_EnemyAnimationManager2->addState(0, frameVector, false, 1.0f);
	frameVector.clear();

	if (!m_TextTexture->loadFromFile("Textures/finalcutscene/ArtByCringenut.png"))
	{
		cout << "Nut credits load failure" << endl;
	}
	m_TextSprite = make_unique<sf::Sprite>();
	m_TextSprite->setTexture(*m_TextTexture);
	m_TextSprite->setOrigin(m_TextTexture->getSize().x / 2, m_TextTexture->getSize().y / 2);
	m_TextSprite->setPosition(640, m_BGUpdatePos + m_CreditsOffset);
}

void FinalCutscene::loadFarout()
{
	m_Backgrounds.clear();
	m_BackgroundSprites.clear();

	addBackground(0.0, 8920, "Textures/space/space_stars_small.png", 9120);

	addBackground(0.0, 8720, "Textures/space/space_stars_big.png", 9120);

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 8, 14));
	frameVector.push_back(sf::IntRect(8, 0, 8, 14));

	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheet.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		

	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheetBlue.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		
	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheetOrange.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		
	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheetRed.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		
	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheetWhite.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		
	for (int i = 0; i < 3; i++)
	{
		addBackgroundElement(sf::Vector2f(((float)(rand() / (float)RAND_MAX)) * 1280, m_BGUpdatePos - 18300), m_BGUpdatePos, "Textures/finalcutscene/PlayerSpriteSheetYellow.png", frameVector, 0.2f, 19200, ((float)(rand() / (float)RAND_MAX)) * 960 + 300);
		if (rand() % 2 == 1)
		{
			m_Backgrounds[m_Backgrounds.size() - 1]->setScale(-2, 2);
		}
	}
		
	frameVector.clear();

	if (!m_TextTexture->loadFromFile("Textures/finalcutscene/ProgrammingAndDesignBySheriffTumbleweed.png"))
	{
		cout << "Me credits load failure" << endl;
	}
	m_TextSprite = make_unique<sf::Sprite>();
	m_TextSprite->setTexture(*m_TextTexture);
	m_TextSprite->setOrigin(m_TextTexture->getSize().x / 2, m_TextTexture->getSize().y / 2);
	m_TextSprite->setPosition(640, m_BGUpdatePos + m_CreditsOffset);
}

void FinalCutscene::UpdateTransitionPos()
{
	m_initialTransitionPosition = sf::Vector2f(0, m_BGUpdatePos - 960);
	m_finalTransitionPosition = sf::Vector2f(0, m_BGUpdatePos);
	m_finalTransitionPosition2 = sf::Vector2f(0, m_BGUpdatePos + (960));
}
