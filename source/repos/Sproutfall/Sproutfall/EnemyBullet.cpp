#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition, int behavior, float velocity)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = Texture;
	m_Sprite->setTexture(*m_Texture);
	m_directionX = direction.x;
	m_directionY = direction.y;
	m_Sprite->setPosition(initialPosition);
	m_Sprite->setRotation(degrees);
	m_Sprite->setScale(2, 2);
	m_Sprite->setOrigin(16, 16);
	configureAnimations();
	setHittable(true);
	m_Hitbox = make_unique<sf::CircleShape>();
	m_Hitbox->setRadius(3);
	m_Hitbox->setScale(m_Sprite->getScale());
	m_Hitbox->setFillColor(sf::Color(0xff0000aa));
	m_Hitbox->setOrigin(m_Hitbox->getGlobalBounds().left + m_Hitbox->getRadius(), m_Hitbox->getGlobalBounds().top + m_Hitbox->getRadius());
	m_behavior = behavior;
	m_Sprite->setRotation(m_Sprite->getRotation() + 90);

	m_speed = velocity;
	//m_speed += (((float)rand() - (RAND_MAX / 2)) / (float)RAND_MAX / 2) * m_randomInitialSpeed;
}

void EnemyBullet::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Colliding state
	frameVector.push_back(sf::IntRect(192, 0, 32, 32));
	frameVector.push_back(sf::IntRect(224, 0, 32, 32));
	frameVector.push_back(sf::IntRect(256, 0, 32, 32));
	m_AnimationManager->addState(colliding, frameVector, false, 0.01f);
	frameVector.clear();

	//Despawning state
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	m_AnimationManager->addState(despawning, frameVector, false, 0.02f);
	frameVector.clear();

	m_CurrentState = neutral;
	m_AnimationManager->setState(neutral);
}

void EnemyBullet::Update(float tf)
{
	m_Sprite->move(m_directionX * m_speed * tf, m_directionY * m_speed * tf);
	m_AnimationManager->Update(tf);
	m_travelTime += tf;
	if (m_behavior == Bullet)
	{
		if (m_travelTime > m_maxTravelTime)
		{
			m_CurrentState = despawning;
			m_AnimationManager->setState(despawning);
			setHittable(false);
		}

		if (m_CurrentState == despawning && !m_AnimationManager->isPlaying())
		{
			m_alive = false;
			m_draw = false;
		}
	}
	else if (m_behavior == ShotgunPellet)
	{
		if (m_speed > 0)
		{
			m_speed -= m_slowdown * tf;
		}
		else
		{
			m_speed = 0;
		}
		if (m_speed <= m_DespawnSpeed && m_CurrentState == neutral)
		{
			m_alive = false;
			m_CurrentState = despawning;
			m_AnimationManager->setState(despawning);
			setHittable(false);
		}
	}
	m_Hitbox->setPosition(m_Sprite->getPosition());
}

bool EnemyBullet::canDespawn()
{
	return (!m_AnimationManager->isPlaying() && !m_alive);
}

void EnemyBullet::killBullet()
{
	m_alive = false;
	m_CurrentState = despawning;
	m_AnimationManager->setState(despawning);
	setHittable(false);
	m_draw = false;
}

sf::CircleShape* EnemyBullet::getHitbox()
{
	return m_Hitbox.get();
}

bool EnemyBullet::checkBulletCollision(sf::CircleShape* PlayerHitbox)
{
	if (m_alive && m_IsHittable)
	{
		if (calculateCollision(m_Hitbox.get(), PlayerHitbox))
		{
			m_AnimationManager->setState(colliding);
			return true;
		}
	}
	return false;
}

void EnemyBullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_draw)
	{
		target.draw(*m_Sprite);
		//target.draw(*m_Hitbox);
	}
}
