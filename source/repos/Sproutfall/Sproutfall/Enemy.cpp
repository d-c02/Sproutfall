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