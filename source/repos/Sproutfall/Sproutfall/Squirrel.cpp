#include "Squirrel.h"

Squirrel::Squirrel(sf::Texture* texture, Player* player, sf::Shader* whiteShader)
{
	m_Player = player;
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(32, 32);
	m_Sprite->setScale(2, 2);
	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(7);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_health = 4;
	m_whiteShader = whiteShader;
	m_blinking = false;
	m_Shootable = true;
	m_IsHittable = true;
	configureAnimations();
}

void Squirrel::Update(float tf)
{
	if (m_health > 0)
	{
		if (m_CurrentState == neutral)
		{

			if (m_Player->getPosition().y > getPosition().y && m_climbingAcceleration < 0)
			{
				m_climbingAcceleration = m_climbingAcceleration * -1;
				m_Sprite->setScale(m_Sprite->getScale().x, -2);
			}
			else if (m_Player->getPosition().y < getPosition().y && m_climbingAcceleration > 0)
			{
				m_climbingAcceleration = m_climbingAcceleration * -1;
				m_Sprite->setScale(m_Sprite->getScale().x, 2);
			}

			float dist = sqrt(pow(m_Player->getPosition().x - getPosition().x, 2) + pow(m_Player->getPosition().y - getPosition().y, 2));
			if (dist < m_attackRange)
			{
				m_CurrentState = jumping;
				m_VelocityX = ((m_Player->getPosition().x - getPosition().x + m_Player->getVelocity().x * tf) / dist) * m_JumpVelocity;
				m_VelocityY = ((m_Player->getPosition().y - getPosition().y + m_Player->getVelocity().y * tf) / dist) * m_JumpVelocity;
				if (m_Player->getPosition().x > getPosition().x)
				{
					m_Sprite->setScale(-2, 2);
				}
				else
				{
					m_Sprite->setScale(2, 2);
				}
				m_AnimationManager->setState(jumping);
			}
			else if (dist < m_runRange)
			{
				m_CurrentState = climbing;
				m_AnimationManager->setState(climbing);
			}
		}
		else if (m_CurrentState == climbing)
		{
			if (m_currentClimbTime < m_climbingSpeedPeakTime * 2)
			{
				if (m_currentClimbTime < m_climbingSpeedPeakTime)
				{
					m_VelocityY += m_climbingAcceleration * tf;
				}
				else
				{
					m_VelocityY -= m_climbingAcceleration * tf;
				}

				m_currentClimbTime += tf;
			}
			else
			{
				m_currentClimbTime = 0.0f;
				m_CurrentState = neutral;
				m_AnimationManager->setState(neutral);
			}

		}
		else if (m_CurrentState == jumping)
		{
			if (m_VelocityY < m_terminalVelocity)
			{
				m_VelocityY += m_gravity * tf;
			}
			else if (m_VelocityY > m_terminalVelocity)
			{
				m_VelocityY += m_airResistance * tf;
			}
			if (m_VelocityX > 0)
			{
				if ((m_airResistanceX * tf) > m_VelocityX)
				{
					m_VelocityX = 0;
				}
				else
					m_VelocityX += m_airResistanceX * tf;
			}
			else if (m_VelocityX < 0)
			{
				if ((m_airResistanceX * tf) < m_VelocityX)
				{
					m_VelocityX = 0;
				}
				else
				m_VelocityX -= m_airResistanceX * tf;
			}
			if ((getPosition().x > 40 && getPosition().x < 170) || (getPosition().x > 470 && getPosition().x < 680) || (getPosition().x > 1080 && getPosition().x < 1250))
			{
				float dist = sqrt(pow(m_Player->getPosition().x - getPosition().x, 2) + pow(m_Player->getPosition().y - getPosition().y, 2));
				if (dist > m_attackRange)
				{
						m_currentJumpTime = 0.0f;
						m_CurrentState = neutral;
						m_VelocityX = 0.0f;
						m_VelocityY = 0.0f;
						m_AnimationManager->setState(neutral);
				}
			}

			m_currentJumpTime += tf;
			if (m_currentJumpTime > m_maxJumpTime)
			{
				m_CurrentState = dead;
				m_AnimationManager->setState(dead);
				m_alive = false;
			}
		}

		if (m_blinking)
		{
			Blink(tf);
		}

		m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
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

void Squirrel::Hurt(sf::Vector2f impactVelocity)
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

void Squirrel::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

void Squirrel::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;

	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 64, 64));
	m_AnimationManager->addState(neutral, frameVector, true, 0.1f);
	frameVector.clear();

	//Climbing state
	frameVector.push_back(sf::IntRect(64, 0, 64, 64));
	frameVector.push_back(sf::IntRect(128, 0, 64, 64));
	frameVector.push_back(sf::IntRect(192, 0, 64, 64));
	frameVector.push_back(sf::IntRect(256, 0, 64, 64));
	frameVector.push_back(sf::IntRect(320, 0, 64, 64));
	m_AnimationManager->addState(climbing, frameVector, true, 0.2f);
	frameVector.clear();

	//Jumping state
	frameVector.push_back(sf::IntRect(384, 0, 64, 64));
	m_AnimationManager->addState(jumping, frameVector, true, 1.0f);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(448, 0, 64, 64));
	m_AnimationManager->addState(dead, frameVector, true, 1.0f);
	frameVector.clear();

	m_CurrentState = neutral;
	m_AnimationManager->setState(neutral);
}
