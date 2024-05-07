#include "BadCloud.h"

BadCloud::BadCloud(sf::Texture* texture, Player* player)
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
	circ->setRadius(20);
	//circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_Shootable = false;
	setHittable(true);

	m_belowCollider = sf::RectangleShape(sf::Vector2f(32 * 2, 128 * 2));
	m_belowCollider.setOrigin(m_belowCollider.getSize().x / 2, 0);
	m_belowCollider.setFillColor(sf::Color::Green);

	m_Player = player;
	m_hasProjectiles = true;
}

void BadCloud::Update(float tf)
{
	m_Hitbox->setPosition(m_Sprite->getPosition());
	m_belowCollider.setPosition(m_Sprite->getPosition().x, m_Sprite->getPosition().y + 16);
}


void BadCloud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_Hitbox);
	target.draw(m_belowCollider);
}

bool BadCloud::checkProjectiles()
{
	if (HitboxIsCircular(m_Player->getHitbox()))
	{
		sf::CircleShape* playerHitbox = dynamic_cast<sf::CircleShape*>(m_Player->getHitbox());
		if (calculateCollision(&m_belowCollider, playerHitbox))
		{
			m_belowCollider.setFillColor(sf::Color::Black);
		}
		else
		{
			m_belowCollider.setFillColor(sf::Color::Green);
		}
	}
	return false;
}