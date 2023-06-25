#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
class Enemy : public Entity, public sf::Drawable
{
public:
	virtual void Update(float tf);
	sf::FloatRect getGlobalBounds();
	virtual void configureAnimations();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void Die();
	bool m_getHittable = true;
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 500;
	float m_health;
};