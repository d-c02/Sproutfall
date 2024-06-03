#pragma once
#include "SFML_Lib.h"
#include "Bullet.h"
class BulletManager : public sf::Drawable
{
public:
	BulletManager();
	~BulletManager();
	void removeBullet(int index);
	void spawnVolley(sf::Vector2f direction, sf::Vector2f initialPos);
	void Update(float tf);
	bool checkCollisions(Enemy* enemy);
	void clearBullets();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<std::unique_ptr<Bullet>> m_bulletVector;
	float m_volleySize = 8;
	std::unique_ptr<sf::Texture> m_bulletTexture;
	float m_spread = 0.3;
};