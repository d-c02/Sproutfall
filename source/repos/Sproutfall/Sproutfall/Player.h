#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include "Reticle.h"
#include "BulletManager.h"
#include "AnimationManager.h"
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
protected:

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_accelerationX = 0;
	float m_accelerationY = 0;
	const float m_terminalVelocity = 300;
	const float m_airResistance = -500;
	const float m_airResistanceX = -250;
	const float m_recoil = -17.5;
	const float m_gravity = 500;
	float m_reloadProgress = 0;
	const int m_bulletsMax = 4;
	int m_bullets = m_bulletsMax;
	const float m_reloadDelay = 1;
	Reticle* m_Reticle;
	float initialPositionX = 200;
	float initialPositionY = 400;
	BulletManager* m_bulletManager;
	AnimationManager* m_AnimationManager;
	sf::IntRect* m_Hitbox;
	enum States {neutral, falling, falling_fast};
	int currentState = neutral;
	vector<sf::Sound*> m_ShotgunShootSounds;
	vector<sf::SoundBuffer*>m_ShotgunShootBuffers;
	float neutralThreshold = 100;
	float fallingThreshold = 600;
	bool m_SpriteFlipped = false;
};