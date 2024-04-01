#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "Asteroid.h"
#include "Squid.h"
class EnemyManager : public sf::Drawable
{
public:
	EnemyManager(sf::Vector2f viewSize, Player* player);
	~EnemyManager();
	void generateEnemies(int type, float minYDistance, float maxYDistance, int numScreens);
	void Update(float tf);
	void Clear();
private:
	void removeEnemy(int index);
	void removeSmoke(int index);
	void AddEnemy(int type, sf::Vector2f position);
	void checkCollisions();
	void addSmoke(sf::Vector2f position);
	float m_ViewSizeX = 0;
	float m_ViewSizeY = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<std::unique_ptr<sf::Texture>> m_EnemyTextures;
	std::vector<std::unique_ptr<Enemy>> m_Enemies;
	enum Behaviors {b_Asteroid, b_Squid, b_Cloud, b_Bird, b_Branch, b_Bug};
	int m_numEnemies = 2;
	Player* m_Player;
	std::vector<std::unique_ptr<sf::Sprite>> m_SmokeSprites;
	std::unique_ptr<sf::Texture> m_smokeTexture;
	std::vector<std::unique_ptr<AnimationManager>> m_SmokeAnimationManagers;
};