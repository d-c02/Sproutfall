#pragma once
#include "SFML_Lib.h"
#include "Bullet.h"
class BulletManager : public sf::Drawable
{
public:
	BulletManager();
	~BulletManager();
	void pushBullet(Bullet* bullet);
	void removeBullet(int index);
	void spawnVolley(sf::Vector2f direction, sf::Vector2f initialPos);
	void Update(float tf);
	//bool checkCollisions(EnemyManager* manager) Implement once enemies are added

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<Bullet*> m_bulletVector;
	float m_volleySize = 5;
	sf::Texture* m_bulletTexture;
	float m_spread = 0.1;
};