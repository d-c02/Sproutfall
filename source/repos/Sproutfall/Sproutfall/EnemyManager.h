#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void AddEnemy(int type);
	void Update(float tf);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<std::unique_ptr<Enemy>> m_Enemies;
	enum Behaviors {Asteroid, Squid, Cloud, Bird, Branch, Bug};
};