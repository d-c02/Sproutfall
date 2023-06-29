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

sf::FloatRect Entity::getGlobalBounds()
{
	return m_Sprite->getGlobalBounds();
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

sf::Sprite* Entity::getSprite()
{
	return m_Sprite.get();
}

bool Entity::calculateCollision(sf::CircleShape* circle, sf::CircleShape* other)
{
	sf::Vector2f diff(circle->getPosition().x - other->getPosition().x, circle->getPosition().y - other->getPosition().y);
	return (sqrt((diff.x * diff.x) + (diff.y * diff.y)) <= circle->getRadius() + other->getRadius());
}



bool Entity::calculateCollision(sf::CircleShape* circle, sf::RectangleShape* rect)
{
	return calculateCollision(rect, circle);
}

bool Entity::calculateCollision(sf::RectangleShape* rect, sf::CircleShape* circle)
{
	float testX = circle->getPosition().x;
	float testY = circle->getPosition().y;
	if (circle->getPosition().x < rect->getPosition().x)
	{
		testX = rect->getPosition().x;
	}
	else if (circle->getPosition().x > rect->getPosition().x + rect->getGlobalBounds().width)
	{
		testX = rect->getPosition().x + rect->getGlobalBounds().width;
	}
	if (circle->getPosition().y < rect->getPosition().y)
	{
		testY = rect->getPosition().y;
	}
	else if (circle->getPosition().y > rect->getPosition().y + rect->getGlobalBounds().height)
	{
		testY = rect->getPosition().y + rect->getGlobalBounds().height;
	}
	float distX = circle->getPosition().x - testX;
	float distY = circle->getPosition().y - testY;
	float distance = sqrt(distX * distX + distY * distY);
	return (distance <= circle->getRadius());
}

bool Entity::calculateCollision(sf::RectangleShape* rect, sf::RectangleShape* other)
{
	return rect->getGlobalBounds().intersects(other->getGlobalBounds());
}

bool Entity::HitboxIsCircular(sf::Shape* shape)
{
	return (typeid(*shape) == typeid(sf::CircleShape));
}

sf::Shape* Entity::getHitbox()
{
	return m_Hitbox.get();
}
//bool Entity::calculateCollision(Entity* entity2)
//{
//	sf::Vector2f spriteSize1(m_Sprite->getLocalBounds().width * m_Sprite->getScale().x, m_Sprite->getLocalBounds().height * m_Sprite->getScale().y);
//	sf::Sprite* sprite2 = entity2->getSprite();
//	sf::Vector2f spriteSize2(sprite2->getLocalBounds().width * sprite2->getScale().x, sprite2->getLocalBounds().height * sprite2->getScale().y);
//	float radius1 = (spriteSize1.x + spriteSize1.y) / 4.0f;
//	float radius2 = (spriteSize2.x + spriteSize2.y) / 4.0f;
//
//	sf::FloatRect sprite1Bounds = m_Sprite->getGlobalBounds();
//	sf::Vector2f spriteCenter1(sprite1Bounds.left + sprite1Bounds.width / 2.0f, sprite1Bounds.top + sprite1Bounds.height / 2.0f);
//
//	sf::FloatRect sprite2Bounds = sprite2->getGlobalBounds();
//	sf::Vector2f spriteCenter2(sprite2Bounds.left + sprite2Bounds.width / 2.0f, sprite2Bounds.top + sprite2Bounds.height / 2.0f);
//	sf::Vector2f difference = spriteCenter1 - spriteCenter2;
//	return (difference.x * difference.x + difference.y * difference.y <= (radius1 + radius2) * (radius1 + radius2));
//}