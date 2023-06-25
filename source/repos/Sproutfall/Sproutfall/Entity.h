#pragma once
#include "SFML_Lib.h"
class Entity
{
public:

	virtual void Update(float tf) = 0;

	//Position
	void setPosition(sf::Vector2f position);

	void setPosition(float x, float y);

	sf::Vector2f getPosition();

	//Size
	void setSize(sf::Vector2f size);

	void setSize(float x, float y);

	sf::Vector2f getSize();

	//Velocity
	void setVelocity(sf::Vector2f velocity);

	void setVelocity(float x, float y);

	sf::Vector2f getVelocity();

	//Hittable
	void setHittable(bool hittable);

	bool getHittable();

	sf::FloatRect getGlobalBounds();
protected:
	std::unique_ptr<sf::Sprite> m_Sprite;
	std::unique_ptr<sf::Texture> m_Texture;

	float m_VelocityX;
	float m_VelocityY;
	bool m_IsHittable;
};