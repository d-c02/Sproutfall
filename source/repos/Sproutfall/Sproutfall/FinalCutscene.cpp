#include "FinalCutscene.h"

FinalCutscene::FinalCutscene(float viewSizeX, float viewSizeY)
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
	frameVector.clear();

	m_State = s_PlayerFalling;
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
		}
	}
	else if (m_State == s_SproutGrowing)
	{
		m_SproutAnimationManager->Update(tf);
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

void FinalCutscene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear(sf::Color(m_BackgroundFillColor));
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
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
	if (m_State == s_SproutGrowing)
	{
		target.draw(*m_SproutSprite);
	}
}