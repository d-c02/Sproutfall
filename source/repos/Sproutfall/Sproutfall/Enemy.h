#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
class Enemy : public Entity, public sf::Drawable
{
public:
	virtual void Update(float tf);
	sf::FloatRect getGlobalBounds();
	virtual void configureAnimations();
	bool GetStatus();
	virtual void Hurt(sf::Vector2f impactVelocity);
	int getHealth();
	void Blink(float tf);
	bool getShootable();
protected:
	float m_health;
	bool m_alive = true;
	bool m_blinking = false;
	bool m_blinkBit = true;
	float m_totalBlinkTime = 0.0f;
	float m_maxTotalBlinkTime = 0.5f;
	float m_blinkDuration = 0.1f;
	float m_currentBlinkDuration = 0.0f;
	bool m_Shootable;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//virtual void Die();
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 500;
};