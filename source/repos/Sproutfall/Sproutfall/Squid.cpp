#include "Squid.h"
Squid::Squid(sf::Texture* texture, Player* player, sf::Shader* whiteShader)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(12, 13);
	m_Sprite->setScale(2, 2);
	configureAnimations();
	m_Player = player;
	setHittable(true);
	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(12);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_health = 4;
	m_whiteShader = whiteShader;
}
Squid::~Squid()
{

}

void Squid::Update(float tf)
{
	if (m_health > 0)
	{
		sf::Vector2f diff((m_Player->getPosition().x /*+ (m_Player->getVelocity().x)*/) - getPosition().x, m_Player->getPosition().y + (m_Player->getVelocity().y / 2) - getPosition().y);
		float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
		if (m_State == neutral)
		{
			if (length <= m_attackRange)
			{
				m_State = startDash;
				m_AnimationManager->setState(m_State);
				m_RotationDegrees = atan2f(diff.y, diff.x) * (180.0 / 3.141592653589793238463);
			}
		}
		else if (m_State != neutral && m_AnimationManager->isPlaying() == false)
		{
			if (m_State == startDash)
			{
				m_State = inDash;
				m_VelocityX = cosf(m_RotationDegrees * (3.141592653589793238463 / 180.0));
				m_VelocityY = sinf(m_RotationDegrees * (3.141592653589793238463 / 180.0));
				float vlength = sqrt(pow(m_VelocityX, 2) + pow(m_VelocityY, 2));
				m_VelocityX = m_VelocityX / vlength * m_Speed;
				m_VelocityY = m_VelocityY / vlength * m_Speed;
			}
			else if (m_State == inDash)
			{
				m_State = endDash;
			}
			else if (m_State == endDash)
			{
				m_State = neutral;
				m_VelocityX = 0;
				m_VelocityY = 0;
			}
			m_AnimationManager->setState(m_State);
		}

		//Velocity calculation stuff here (from player)
		m_AnimationManager->Update(tf);
		if (length <= m_attackRange)
		{
			m_Sprite->setRotation(m_RotationDegrees + 90);
		}
		else
		{
			m_Sprite->setRotation(m_RotationDegrees);
		}
		m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
		m_Hitbox->setPosition(m_Sprite->getPosition());

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

}

void Squid::Hurt(sf::Vector2f impactVelocity)
{
	m_blinking = true;
	m_currentBlinkDuration = 0.0f;
	m_totalBlinkTime = 0.0f;
	m_health--;
	if (m_health <= 0)
	{
		setHittable(false);
		m_Sprite->setRotation(0);
		m_VelocityX = impactVelocity.x / 2;
		m_VelocityY = impactVelocity.y / 2;
		m_AnimationManager->setState(dead);
	}
}

void Squid::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(173, 0, 25, 26));
	frameVector.push_back(sf::IntRect(199, 0, 26, 26));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Start dashing state
	frameVector.push_back(sf::IntRect(0, 0, 25, 25));
	m_AnimationManager->addState(startDash, frameVector, false, 0.25f);
	frameVector.clear();

	//In dash state
	frameVector.push_back(sf::IntRect(53, 0, 18, 35));
	frameVector.push_back(sf::IntRect(71, 0, 18, 31));
	frameVector.push_back(sf::IntRect(71, 0, 18, 31));
	frameVector.push_back(sf::IntRect(71, 0, 18, 31));
	m_AnimationManager->addState(inDash, frameVector, false, 0.025f);
	frameVector.clear();

	//End dashing state
	frameVector.push_back(sf::IntRect(125, 0, 23, 32));
	frameVector.push_back(sf::IntRect(148, 0, 24, 29));
	m_AnimationManager->addState(endDash, frameVector, false, 0.5f);
	frameVector.clear();
	
	//Dead state
	frameVector.push_back(sf::IntRect(225, 0, 27, 17));
	m_AnimationManager->addState(dead, frameVector, true, 1.0f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);
}

void Squid::draw(sf::RenderTarget& target, sf::RenderStates states) const
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