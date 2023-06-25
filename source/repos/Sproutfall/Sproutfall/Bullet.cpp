#include "Bullet.h"
Bullet::Bullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = Texture;
	m_Sprite->setTexture(*m_Texture);
	m_directionX = direction.x;
	m_directionY = direction.y;
	m_Sprite->setPosition(initialPosition);
	m_Sprite->setRotation(degrees);
}

Bullet::~Bullet()
{

}

void Bullet::Update(float tf)
{
	m_Sprite->move(m_directionX * m_speed * tf, m_directionY * m_speed * tf);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}