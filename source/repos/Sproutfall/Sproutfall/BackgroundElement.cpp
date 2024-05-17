#include "BackgroundElement.h"
BackgroundElement::BackgroundElement(Player* player, sf::Vector2f position, float parallaxSpeed, string texturePath, vector<sf::IntRect> frameVector, float frameDelay, bool holdFlush, float vertHoldBoundary)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile(texturePath))
	{
		cout << "Animation element texture load failure";
	}
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(position);
	m_Sprite->setTexture(*m_Texture);
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	m_AnimationManager->addState(0, frameVector, true, frameDelay);
	m_Player = player;
	m_ParallaxSpeed = parallaxSpeed;
	m_holdFlush = holdFlush;
	m_holdVert = vertHoldBoundary;
}

void BackgroundElement::Update(float tf)
{
	if (m_holdFlush && m_Player->getPosition().y - 480 > m_Sprite->getPosition().y - m_holdVert)
	{
		
		m_Sprite->setPosition(m_Sprite->getPosition().x, m_Player->getPosition().y - 480 + m_holdVert);
		m_Sprite->move(0, m_Player->getVelocity().y * tf);
	}
	else
	{
		m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed);
	}
	m_AnimationManager->Update(tf);
}

void BackgroundElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite.get());
}

void BackgroundElement::setScale(float x, float y)
{
	m_Sprite->setScale(x, y);
}

void BackgroundElement::setPosition(float x, float y)
{
	m_Sprite->setPosition(x, y);
}

