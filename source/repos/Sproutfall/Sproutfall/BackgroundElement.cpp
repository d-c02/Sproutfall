#include "BackgroundElement.h"
BackgroundElement::BackgroundElement(Player* player, float parallaxSpeed, string texturePath, AnimationManager animationManager)
{
	m_Texture = make_unique<sf::Texture>();
	m_Sprite = make_unique<sf::Sprite>();
	m_Player = player;
	m_ParallaxSpeed = parallaxSpeed;
}

void BackgroundElement::Update(float tf)
{
	m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
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

