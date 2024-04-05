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
	m_Sprite->setScale(2, 2);
	m_Sprite->setOrigin(10, 3);
	configureAnimations();
	setHittable(true);
	m_Hitbox = make_unique<sf::CircleShape>();
	m_Hitbox->setRadius(3);
	m_Hitbox->setScale(m_Sprite->getScale());
	m_Hitbox->setFillColor(sf::Color(0xff0000aa));
	m_Hitbox->setOrigin(m_Hitbox->getGlobalBounds().left + m_Hitbox->getRadius(), m_Hitbox->getGlobalBounds().top + m_Hitbox->getRadius());
}

Bullet::~Bullet()
{

}

sf::CircleShape* Bullet::getHitbox()
{
	return m_Hitbox.get();
}

void Bullet::Update(float tf)
{
	m_Sprite->move(m_directionX * m_speed * tf, m_directionY * m_speed * tf);
	m_AnimationManager->Update(tf);
	m_travelTime += tf;
	if (m_travelTime >= m_maxTravelTime && m_CurrentState == neutral)
	{
		m_Alive = false;
		m_CurrentState = despawning;
		m_AnimationManager->setState(despawning);
		setHittable(false);
	}
	m_Hitbox->setPosition(m_Sprite->getPosition());
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	//target.draw(*m_Hitbox);
}

void Bullet::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 13, 6));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Colliding state
	frameVector.push_back(sf::IntRect(50, 0, 13, 18));
	frameVector.push_back(sf::IntRect(63, 0, 13, 20));
	frameVector.push_back(sf::IntRect(74, 0, 8, 26));
	m_AnimationManager->addState(colliding, frameVector, false, 0.02f);
	frameVector.clear();

	//Despawning state
	frameVector.push_back(sf::IntRect(13, 0, 9, 6));
	frameVector.push_back(sf::IntRect(22, 0, 6, 5));
	frameVector.push_back(sf::IntRect(28, 0, 6, 5));
	frameVector.push_back(sf::IntRect(34, 0, 8, 7));
	frameVector.push_back(sf::IntRect(42, 0, 8, 7));
	m_AnimationManager->addState(despawning, frameVector, false, 0.1f);
	frameVector.clear();

	m_CurrentState = neutral;
	m_AnimationManager->setState(neutral);
}

void Bullet::checkCollision(Enemy* enemy)
{
	
	if (m_IsHittable)
	{
		if (HitboxIsCircular(enemy->getHitbox()))
		{
			sf::CircleShape* enemyHitbox = dynamic_cast<sf::CircleShape*>(enemy->getHitbox());
			if (calculateCollision(m_Hitbox.get(), enemyHitbox))
			{
				//enemyHitbox->setFillColor(sf::Color(0x00ff00aa));
				m_CurrentState = colliding;
				m_Alive = false;
				m_AnimationManager->setState(colliding);
				m_Sprite->setScale(1.5f, 1.5f);
				setHittable(false);
				enemy->Hurt(sf::Vector2f(m_directionX * m_speed, m_directionY * m_speed));
				m_speed = 0;
			}
		}
		else
		{
			sf::RectangleShape* enemyHitbox = dynamic_cast<sf::RectangleShape*>(enemy->getHitbox());
			if (calculateCollision(m_Hitbox.get(), enemyHitbox))
			{
				//enemyHitbox->setFillColor(sf::Color(0x00ff00aa));
				m_CurrentState = colliding;
				m_Alive = false;
				m_AnimationManager->setState(colliding);
				m_Sprite->setScale(1.5f, 1.5f);
				setHittable(false);
				enemy->Hurt(sf::Vector2f(m_directionX * m_speed, m_directionY * m_speed));
				m_speed = 0;
			}
		}
	}
}

bool Bullet::canDespawn()
{
	return (!m_AnimationManager->isPlaying() && !m_Alive);
}