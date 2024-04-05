#include "Enemy.h"

void Enemy::Update(float tf)
{

}
sf::FloatRect Enemy::getGlobalBounds()
{
	return m_Sprite->getGlobalBounds();
}

void Enemy::configureAnimations()
{

}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}

bool Enemy::GetStatus()
{
	return m_alive;
}

void Enemy::Hurt()
{
	m_blinking = true;
	m_currentBlinkDuration = 0.0f;
	m_totalBlinkTime = 0.0f;
	m_health--;
}

void Enemy::Blink(float tf)
{
	m_currentBlinkDuration += tf;
	m_totalBlinkTime += tf;
	if (m_currentBlinkDuration > m_blinkDuration)
	{
		m_blinkBit = !m_blinkBit;
		m_currentBlinkDuration = 0.0f;
	}
	if (m_totalBlinkTime > m_maxTotalBlinkTime)
	{
		m_blinkBit = true;
		m_currentBlinkDuration = 0.0f;
		m_totalBlinkTime = 0.0f;
		m_blinking = false;
	}
}

int Enemy::getHealth()
{
	return m_health;
}