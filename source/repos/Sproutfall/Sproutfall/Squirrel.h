#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"

class Squirrel : public sf::Drawable, public Enemy
{
public:
	Squirrel(sf::Texture* texture, Player* player, sf::Shader* whiteShader);
	void Update(float tf);
	void Hurt(sf::Vector2f impactVelocity);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void configureAnimations();
	std::unique_ptr<AnimationManager> m_AnimationManager;
	const float m_terminalVelocity = 800;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 500;
	const float m_JumpVelocity = 750;
	enum States { neutral, climbing, jumping, dead };
	Player* m_Player;
	const float m_attackRange = 500;
	const float m_runRange = 2000;
	sf::Shader* m_whiteShader;
	float m_currentDeathTime = 0.0f;
	const float m_deathTime = 0.5f;
	int m_CurrentState;

	float m_climbingSpeedPeakTime = 0.5f;
	float m_currentClimbTime = 0.0f;
	float m_climbingAcceleration = -500.0f;

	float m_maxJumpTime = 5.0f;
	float m_currentJumpTime = 0.0f;
};