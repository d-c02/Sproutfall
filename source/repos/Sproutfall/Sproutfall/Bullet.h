#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include "AnimationManager.h"
#include "Enemy.h"
class Bullet : public Entity, public sf::Drawable
{
public:
	Bullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition);
	~Bullet();
	void Update(float tf);
	void configureAnimations();
	void checkCollision(Enemy* enemy);
	bool canDespawn();
private:
	sf::Texture* m_Texture;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	enum States {neutral, colliding, despawning};
	float m_speed = 750;
	float m_directionX;
	float m_directionY;
	bool m_Alive = true;
	int m_CurrentState;
};