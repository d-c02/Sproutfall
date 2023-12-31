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
	void checkCollisions(Enemy* enemy);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<std::unique_ptr<Bullet>> m_bulletVector;
	float m_volleySize = 5;
	std::unique_ptr<sf::Texture> m_bulletTexture;
	float m_spread = 0.1;
};