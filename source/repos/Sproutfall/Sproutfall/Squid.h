#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"
class Squid : public sf::Drawable, public Enemy
{
public:
	Squid(sf::Texture* texture, Player* player, sf::Shader* whiteShader);
	~Squid();
	void Update(float tf);
	void configureAnimations();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	float m_RotationDegrees;
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	const float m_gravity = 250;
	const float m_Speed = 500;
	enum States {neutral, startDash, inDash, endDash};
	Player* m_Player;
	const float m_attackRange = 400;
	int m_State = neutral;
	sf::Shader* m_whiteShader;
	//std::unique_ptr<sf::CircleShape> m_Hitbox;

};