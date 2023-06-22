#include "Entity.h"


//Position
void Entity::setPosition(sf::Vector2f position)
{
	m_Sprite->setPosition(position);
}

void Entity::setPosition(float x, float y)
{
	m_Sprite->setPosition(x, y);
}

sf::Vector2f Entity::getPosition()
{
	return m_Sprite->getPosition();
}

//Size
void Entity::setSize(sf::Vector2f size)
{
	m_Sprite->setScale(size);
}

void Entity::setSize(float x, float y)
{
	m_Sprite->setScale(x, y);
}

sf::Vector2f Entity::getSize()
{
	return m_Sprite->getScale();
}

//Velocity
void Entity::setVelocity(sf::Vector2f velocity)
{
	m_VelocityX = velocity.x;
	m_VelocityY = velocity.y;
}

void Entity::setVelocity(float x, float y)
{
	m_VelocityX = x;
	m_VelocityY = y;
}

sf::Vector2f Entity::getVelocity()
{
	return sf::Vector2f(m_VelocityX, m_VelocityY);
}

//Hittable
void Entity::setHittable(bool hittable)
{
	m_IsHittable = hittable;
}

bool Entity::getHittable()
{
	return m_IsHittable;
}