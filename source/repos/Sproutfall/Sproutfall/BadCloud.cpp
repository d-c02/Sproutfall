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
	m_belowCollider.setFillColor(sf::Color::Blue);

	m_Player = player;
	m_hasProjectiles = true;
	configureAnimations();
	m_State = neutral;
	m_hasSmoke = false;
}

void BadCloud::Update(float tf)
{
	if (m_State == attacking && !m_AnimationManager->isPlaying())
	{
		m_alive = false;
	}
	m_Hitbox->setPosition(m_Sprite->getPosition());
	m_belowCollider.setPosition(m_Sprite->getPosition().x, m_Sprite->getPosition().y + 16);
	m_AnimationManager->Update(tf);
}


void BadCloud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	//target.draw(*m_Hitbox);
	//target.draw(m_belowCollider);
}

bool BadCloud::checkProjectiles()
{
	if (HitboxIsCircular(m_Player->getHitbox()))
	{
		sf::CircleShape* playerHitbox = dynamic_cast<sf::CircleShape*>(m_Player->getHitbox());
		if (calculateCollision(&m_belowCollider, playerHitbox) && m_State == neutral)
		{
			m_AnimationManager->setState(attacking);
			m_State = attacking;
		}
	}
	return false;
}

void BadCloud::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());

	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	m_AnimationManager->addState(neutral, frameVector, true, 0.5f);
	frameVector.clear();

	//Attacking state
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	frameVector.push_back(sf::IntRect(192, 0, 32, 32));
	frameVector.push_back(sf::IntRect(224, 0, 32, 32));
	frameVector.push_back(sf::IntRect(256, 0, 32, 32));
	frameVector.push_back(sf::IntRect(288, 0, 32, 32));
	frameVector.push_back(sf::IntRect(320, 0, 32, 32));
	frameVector.push_back(sf::IntRect(352, 0, 32, 32));
	frameVector.push_back(sf::IntRect(384, 0, 32, 32));
	frameVector.push_back(sf::IntRect(416, 0, 32, 32));
	m_AnimationManager->addState(attacking, frameVector, false, 0.1f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);
}