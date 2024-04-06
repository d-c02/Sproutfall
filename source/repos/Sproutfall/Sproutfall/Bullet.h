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
	sf::CircleShape* getHitbox();
private:
	sf::Texture* m_Texture;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	enum States {neutral, colliding, despawning};
	float m_speed = 1000;
	float m_slowdown = 2000;
	float m_randomInitialSpeed = 100;
	float m_directionX;
	float m_directionY;
	float m_travelTime = 0;
	float m_maxTravelTime = 0.2;
	float m_DespawnSpeed = 100;
	bool m_Alive = true;
	int m_CurrentState;
	std::unique_ptr<sf::CircleShape> m_Hitbox;
};