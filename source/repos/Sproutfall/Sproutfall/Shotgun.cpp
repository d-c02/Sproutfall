#include "Shotgun.h"


Shotgun::Shotgun(sf::Sprite* playerSprite, sf::RenderWindow* window)
{
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile("Textures/Player/shotgun-Sheet.png"))
	{
		cout << "Shotgun texture load failure";
	}
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);
	m_Sprite->setOrigin(8, 8);
	m_playerSprite = playerSprite;
	m_window = window;
	configureAnimations();
}

Shotgun::~Shotgun()
{

}
void Shotgun::Update(float tf)
{
	float xpos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)).x - m_playerSprite->getPosition().x;
	float ypos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)).y - m_playerSprite->getPosition().y;
	float scalingFactor = m_distanceFromPlayer;
	float rads = atan(ypos / xpos);
	float rotationMod = 0;
	if (xpos < 0)
	{
		scalingFactor = scalingFactor * -1;
		rotationMod = 180;
		m_Sprite->setScale(2, -2);
	}
	else
	{
		m_Sprite->setScale(2, 2);
	}
	float tmpx = m_playerSprite->getPosition().x + (scalingFactor * cos(rads));
	float tmpy = m_playerSprite->getPosition().y + (scalingFactor * sin(rads));
	m_Sprite->setPosition(tmpx, tmpy);
	m_Sprite->setRotation(rotationMod + (rads * (180.0 / 3.141592653589793238463)));


	m_AnimationManager->Update(tf);
	if (!m_AnimationManager->isPlaying())
	{
		m_AnimationManager->setState(neutral);
	}
}

void Shotgun::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}

void Shotgun::Shoot()
{
	m_AnimationManager->setState(shooting);
}

void Shotgun::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;

	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 16, 16));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Shooting state
	frameVector.push_back(sf::IntRect(16, 0, 16, 16));
	frameVector.push_back(sf::IntRect(32, 0, 16, 16));
	frameVector.push_back(sf::IntRect(48, 0, 16, 16));
	m_AnimationManager->addState(shooting, frameVector, false, 0.1f);
	frameVector.clear();

	m_Sprite->setTextureRect(sf::IntRect(0, 0, 16, 16));
	m_AnimationManager->setState(neutral);
}