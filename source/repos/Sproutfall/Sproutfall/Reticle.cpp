#include "Reticle.h"


Reticle::Reticle(sf::Sprite* playerSprite, sf::RenderWindow* window)
{
	m_Texture = new sf::Texture();
	if (!m_Texture->loadFromFile("Textures/Reticle.png"))
	{
		cout << "Reticle texture load failure";
	}
	m_Sprite = new sf::Sprite();
	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);
	m_Sprite->setTextureRect(m_animList[m_animIndex]);
	m_Sprite->setOrigin(4, 2);
	m_playerSprite = playerSprite;
	m_window = window;
}

Reticle::~Reticle()
{
	delete(m_Texture);
	delete(m_Sprite);
}
void Reticle::Update(float tf)
{
	double xpos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)).x - m_playerSprite->getPosition().x;
	double ypos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)).y - m_playerSprite->getPosition().y;
	float scalingFactor = m_distanceFromPlayer;
	if (xpos != 0)
	{
		double rads = atan(ypos / xpos);
		double rotationMod = 0;
		if (xpos < 0)
		{
			scalingFactor = scalingFactor * -1;
			rotationMod = 180;
		}
		double tmpx = m_playerSprite->getPosition().x + (scalingFactor * cos(rads));
		double tmpy = m_playerSprite->getPosition().y + (scalingFactor * sin(rads));
		//cout << tmpx << " " << tmpy << "\n";
		//cout << cos(rads) << " " << sin(rads) << "\n";
		//cout << rads << "\n";
		m_Sprite->setPosition(tmpx, tmpy);
		m_Sprite->setRotation(rotationMod + (rads * (180.0 / 3.141592653589793238463) + 90));
	}
	else
	{
		m_Sprite->setPosition(m_playerSprite->getPosition().x, (m_playerSprite->getPosition().y + 1) * scalingFactor);
	}
	m_animCounter += tf;
	if (m_animCounter >= 1)
	{
		if (m_animIndex == 0)
		{
			m_animIndex = 1;
		}
		else
		{
			m_animIndex = 0;
		}
		m_Sprite->setTextureRect(m_animList[m_animIndex]);
		m_animCounter = 0;
	}
}

void Reticle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}