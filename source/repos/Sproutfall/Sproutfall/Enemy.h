#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
class Enemy : public Entity
{
public:
	virtual void Update(float tf);
	sf::FloatRect getGlobalBounds();
	virtual void configureAnimations();
private:
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 500;
};