#include "Shell.h"

Shell::Shell(sf::Texture* Texture, sf::Color color, float gravity, sf::Vector2f initialPosition, sf::Vector2f direction)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setOrigin(6, 6);
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(initialPosition);
	m_Texture = Texture;
	m_Sprite->setTexture(*Texture);
	float Velocity = 400 + rand() % 100;
	m_VelocityX = direction.x * Velocity;
	m_VelocityY = direction.y * Velocity;
	m_Sprite->setColor(color);
	m_gravity = gravity;
	configureAnimations();
}

void Shell::Update(float tf)
{
	m_aliveTime += tf;
	if (m_aliveTime > m_lifetime)
	{
		m_Alive = false;
	}
	if (m_VelocityY < m_terminalVelocity)
	{
		m_VelocityY += m_gravity * tf;
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
	m_AnimationManager->Update(tf);
	m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
}

void Shell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}

bool Shell::CanDespawn()
{
	return !m_Alive;
}

void Shell::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());

	vector<sf::IntRect> frameVector;

	frameVector.push_back(sf::IntRect(0, 0, 12, 12));
	frameVector.push_back(sf::IntRect(12, 0, 12, 12));
	frameVector.push_back(sf::IntRect(24, 0, 12, 12));
	frameVector.push_back(sf::IntRect(36, 0, 12, 12));
	frameVector.push_back(sf::IntRect(48, 0, 12, 12));
	frameVector.push_back(sf::IntRect(60, 0, 12, 12));
	frameVector.push_back(sf::IntRect(72, 0, 12, 12));
	frameVector.push_back(sf::IntRect(84, 0, 12, 12));

	m_AnimationManager->addState(0, frameVector, true, 0.1f);
	frameVector.clear();

	m_AnimationManager->setState(0);
}