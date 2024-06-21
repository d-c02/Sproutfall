#pragma once
#include "SFML_Lib.h"
#include "EnemyBullet.h"

class EnemyBulletManager : public sf::Drawable
{
public:
	EnemyBulletManager(sf::Texture* bulletTexture);
	void removeBullet(int index);
	void spawnVolley(sf::Vector2f direction, sf::Vector2f initialPos);
	void spawnPineconeExplosion(sf::Vector2f initialPos);
	void spawnBullet(sf::Vector2f direction, sf::Vector2f initialPos);
	void Update(float tf);
	bool checkCollisions(Player* player);
	void clearBullets();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<std::unique_ptr<EnemyBullet>> m_bulletVector;
	float m_volleySize = 8;
	sf::Texture* m_bulletTexture;
	float m_spread = 0.3;
	float m_shotgunPelletSpeed = 1000;
	float m_randomInitialPelletSpeed = 100;

	float m_PineconeExplosionSpeed = 500;
	float m_PineconeExplosionTravelTime = 1.0f;
	enum Behaviors { Bullet, ShotgunPellet };
};