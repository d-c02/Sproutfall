#include "Bird.h"

Bird::Bird(sf::Texture* texture, Player* player, sf::Shader* whiteShader)
{
	m_Player = player;
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);
	m_Sprite->setScale(2, 2);
	configureAnimations();
	setHittable(true);
	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(7);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_health = 3;
	m_whiteShader = whiteShader;
	m_blinking = false;
	m_VelocityX = 350;
	m_VelocityY = 210;
	m_Shootable = true;
}

void Bird::Update(float tf)
{
	if (m_health > 0)
	{
		sf::Vector2f diff((m_Player->getPosition().x /*+ (m_Player->getVelocity().x)*/) - getPosition().x, m_Player->getPosition().y - getPosition().y);
		float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
		if (m_State == neutral)
		{
			if (length <= m_attackRange)
			{
				m_State = chasing;
				m_AnimationManager->setState(m_State);
			}
		}
		else if (m_State == chasing)
		{
			if (length > m_attackRange)
			{
				m_State = neutral;
				m_AnimationManager->setState(m_State);
			}
			if (diff.x < 0)
			{
				m_Sprite->setScale(-2, 2);
			}
			else
			{
				m_Sprite->setScale(2, 2);
			}
			m_Sprite->move(m_VelocityX * diff.x / length * tf, m_VelocityY * diff.y / length * tf);
		}

		if (m_blinking)
		{
			Blink(tf);
		}
	}
	else
	{
		m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
		m_AnimationManager->Update(tf);
		m_currentDeathTime += tf;
		if (m_currentDeathTime > m_deathTime)
		{
			m_alive = false;
		}
	}
	m_Hitbox->setPosition(m_Sprite->getPosition());
	m_AnimationManager->Update(tf);
}

void Bird::Hurt(sf::Vector2f impactVelocity)
{
	m_blinking = true;
	m_currentBlinkDuration = 0.0f;
	m_totalBlinkTime = 0.0f;
	m_health--;
	if (m_health <= 0)
	{
		m_Sprite->setScale(2, 2);
		setHittable(false);
		m_Shootable = false;
		m_Sprite->setRotation(0);
		m_VelocityX = impactVelocity.x / 2;
		m_VelocityY = impactVelocity.y / 2;
		m_AnimationManager->setState(dead);
	}
}

void Bird::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	m_AnimationManager->addState(neutral, frameVector, true, 0.1f + ((float) rand() / RAND_MAX) / 20);
	frameVector.clear();

	//Dead state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	m_AnimationManager->addState(chasing, frameVector, true, 0.1f);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(192, 0, 32, 32));
	m_AnimationManager->addState(dead, frameVector, true, 1.0f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);
}

void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_blinking && m_health > 0)
	{
		if (m_blinkBit)
		{
			target.draw(*m_Sprite, m_whiteShader);
		}
		else
		{
			target.draw(*m_Sprite);
		}
	}
	else
	{
		target.draw(*m_Sprite);
	}
	//target.draw(*m_Hitbox);
}
