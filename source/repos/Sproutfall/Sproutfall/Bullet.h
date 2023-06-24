#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
class Bullet : public Entity, public sf::Drawable
{
public:
	Bullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition);
	~Bullet();
	void Update(float tf);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_speed = 750;
	float m_directionX;
	float m_directionY;
};