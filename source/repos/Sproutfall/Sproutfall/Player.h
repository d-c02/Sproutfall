#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include"Enemy.h"
#include "Reticle.h"
#include "BulletManager.h"
#include "AnimationManager.h"
#include "ShellManager.h"

class Player : public sf::Drawable, public Entity
{
public:
	Player(sf::RenderWindow* window);
	~Player();
	void Shoot();
	void Update(float tf);
	void handleInput(sf::Event* event);
	sf::FloatRect getGlobalBounds();
	void configureAnimations();
	void setDirection();
	void CheckCollisions(Enemy* enemy);
	void Die();
	bool getStatus();
	sf::Vector2f getPosition();
protected:

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_accelerationX = 0;
	float m_accelerationY = 0;
	const float m_terminalVelocity = 100;
	const float m_airResistance = -500;
	const float m_airResistanceX = -250;
	const float m_recoil = -17.5;
	const float m_gravity = 500;
	float m_reloadProgress = 0;
	const int m_bulletsMax = 4;
	int m_bullets = m_bulletsMax;
	const float m_reloadDelay = 0.75;
	std::unique_ptr<Reticle> m_Reticle;
	float m_initialPositionX = 640;
	float m_initialPositionY = 200;
	std::unique_ptr<BulletManager> m_bulletManager;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	//sf::IntRect* m_Hitbox;
	enum States {neutral, falling, falling_fast, hurt, dead};
	int m_CurrentState = neutral;
	vector<std::unique_ptr<sf::Sound>> m_ShotgunShootSounds;
	vector<std::unique_ptr<sf::SoundBuffer>>m_ShotgunShootBuffers;
	float neutralThreshold = 90;
	float fallingThreshold = 600;
	bool m_SpriteFlipped = false;
	std::unique_ptr<sf::CircleShape> m_Hitbox;
	const float m_CollisionSlowdown = 0.2;
	int m_Health = 3;
	sf::RenderWindow* m_RenderWindow;
	bool m_Alive = true;
	std::unique_ptr<ShellManager> m_ShellManager;
};