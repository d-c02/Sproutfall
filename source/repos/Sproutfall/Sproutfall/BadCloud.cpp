#include "BadCloud.h"

BadCloud::BadCloud(sf::Texture* texture)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);

	if (rand() % 2 == 0)
	{
		m_Sprite->setScale(2, 2);
	}
	else
	{
		m_Sprite->setScale(-2, 2);
	}

	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(10);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_Shootable = false;
	setHittable(true);
}

void BadCloud::Update(float tf)
{
	m_Hitbox->setPosition(m_Sprite->getPosition());
}

void BadCloud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	//target.draw(*m_Hitbox);
}