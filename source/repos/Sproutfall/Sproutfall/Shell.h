#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include "AnimationManager.h"

class Shell : public sf::Drawable, public Entity
{
public:
	Shell(sf::Texture* Texture, sf::Color color, float gravity, sf::Vector2f initialPosition, sf::Vector2f direction);
	void Update(float tf);
	bool CanDespawn();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void configureAnimations();
	sf::Texture* m_Texture;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	const float m_lifetime = 0.5f;
	const float m_terminalVelocity = 100;
	const float m_airResistanceX = -250;
	float m_aliveTime;
	bool m_Alive = true;
	float m_gravity;
};