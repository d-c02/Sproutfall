#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include "AnimationManager.h"
#include "Player.h"

class EnemyBullet : public Entity, public sf::Drawable
{
public:
	EnemyBullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition, int behavior, float velocity);
	void configureAnimations();

	void Update(float tf);
	bool canDespawn();
	void killBullet();
	sf::CircleShape* getHitbox();
	bool checkBulletCollision(sf::CircleShape* PlayerHitbox);
private:
	sf::Texture* m_Texture;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	enum States { neutral, colliding, despawning };
	float m_speed = 1000;
	float m_slowdown = 2000;
	float m_randomInitialSpeed = 100;
	float m_directionX;
	float m_directionY;
	float m_travelTime = 0;
	float m_maxTravelTime = 0.2;
	float m_DespawnSpeed = 100;
	int m_CurrentState;
	std::unique_ptr<sf::CircleShape> m_Hitbox;
	bool m_draw = true;
	bool m_alive = true;
	int m_behavior;
	enum Behaviors {Bullet, ShotgunPellet};
};