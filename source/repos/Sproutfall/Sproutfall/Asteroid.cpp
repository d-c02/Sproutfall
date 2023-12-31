#include "Asteroid.h"

Asteroid::Asteroid(sf::Texture* texture, Player* player)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_VelocityX = ((float)rand() / (float)(RAND_MAX)) * m_MaxVelocityX;
	m_VelocityY = ((float)rand() / (float)(RAND_MAX)) * m_MaxVelocityY;
	m_RotationDegrees = ((float)rand() / (float)(RAND_MAX)) * m_MaxRotation;
	if (rand() % 2 > 0)
	{
		m_VelocityX *= -1;
	}
	if (rand() % 2 > 0)
	{
		m_VelocityY *= -1;
	}
	if (rand() % 2 > 0)
	{
		m_RotationDegrees *= -1;
	}
	m_Sprite->setOrigin(15, 13);
	m_Sprite->setScale(2, 2);
	setHittable(true);
	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(15);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_health = 8;
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(float tf)
{
	if (m_Sprite->getGlobalBounds().intersects(sf::FloatRect(-1, -10000, 1, 10000000)) || m_Sprite->getGlobalBounds().intersects(sf::FloatRect(1279, -10000, 1, 10000000)))
	{
		m_VelocityX = m_VelocityX * -1;
		// m_RotationDegrees = m_RotationDegrees * -1;
	}
	m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
	m_Sprite->rotate(m_RotationDegrees * tf);
	m_Hitbox->setPosition(m_Sprite->getPosition());
}

void Asteroid::configureAnimations()
{

}

void Asteroid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	//target.draw(*m_Hitbox);
}