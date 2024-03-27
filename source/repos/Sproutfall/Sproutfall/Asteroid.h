#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "Player.h"
class Asteroid : public Enemy, public sf::Drawable
{
public:
	Asteroid(sf::Texture* texture, Player* player);
	~Asteroid();
	void Update(float tf);
	void configureAnimations();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_RotationDegrees;
	const float m_MaxRotation = 15;
	const float m_MaxVelocityX = 300;
	const float m_MaxVelocityY = 50;
	const float m_terminalVelocity = 0;
	const float m_airResistance = -250;
	const float m_airResistanceX = -250;
	bool m_CollidingRebounding = false;
	std::unique_ptr <sf::RectangleShape> m_LeftBound;
	std::unique_ptr <sf::RectangleShape> m_RightBound;
};