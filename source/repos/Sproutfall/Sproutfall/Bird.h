#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"

class Bird : public sf::Drawable, public Enemy
{
public:
	Bird(sf::Texture* texture, Player* player, sf::Shader* whiteShader);
	void Update(float tf);
	void Hurt(sf::Vector2f impactVelocity);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void configureAnimations();
	std::unique_ptr<AnimationManager> m_AnimationManager;
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 250;
	const float m_Speed = 500;
	enum States { neutral, chasing, dead};
	Player* m_Player;
	const float m_attackRange = 400;
	int m_State = neutral;
	sf::Shader* m_whiteShader;
	float m_currentDeathTime = 0.0f;
	const float m_deathTime = 0.5f;
};