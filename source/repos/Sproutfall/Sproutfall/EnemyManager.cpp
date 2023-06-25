#include "EnemyManager.h"

EnemyManager::EnemyManager(sf::Vector2f viewSize, Player* player)
{
	//m_numEnemies = 1 right now, change as enemies are added
	for (int i = 0; i < m_numEnemies; i++)
	{
		m_EnemyTextures.push_back(make_unique<sf::Texture>());
		if (i == b_Asteroid)
		{
			if (!m_EnemyTextures[b_Asteroid]->loadFromFile("Textures/Enemies/asteroid.png"))
			{
				cout << "Textures/Enemies/asteroid.png load failure" << endl;
			}
		}
		else if (i == b_Squid)
		{
			if (!m_EnemyTextures[b_Squid]->loadFromFile("Textures/Enemies/squid.png"))
			{
				cout << "Textures/Enemies/squid.png load failure" << endl;
			}
		}
		else if (i == b_Cloud)
		{

		}
		else if (i == b_Bird)
		{

		}
		else if (i == b_Branch)
		{

		}
		else if (i == b_Bug)
		{

		}
	}
	m_ViewSizeX = viewSize.x;
	m_ViewSizeY = viewSize.y;
	m_Player = player;
}
EnemyManager::~EnemyManager()
{

}
void EnemyManager::AddEnemy(int type, sf::Vector2f position)
{
	if (type == b_Asteroid)
	{
		m_Enemies.push_back(make_unique<Asteroid>(m_EnemyTextures[b_Asteroid].get(), m_Player));
		m_Enemies[m_Enemies.size() - 1]->setPosition(position);
	}
	else if (type == b_Squid)
	{
		m_Enemies.push_back(make_unique<Squid>(m_EnemyTextures[b_Squid].get(), m_Player));
		m_Enemies[m_Enemies.size() - 1]->setPosition(position);
	}
	else if (type == b_Cloud)
	{

	}
	else if (type == b_Bird)
	{

	}
	else if (type == b_Branch)
	{

	}
	else if (type == b_Bug)
	{

	}
}

void EnemyManager::generateEnemies(int type, float minYDistance, float maxYDistance, int numScreens)
{
	int currentScreen = 0;
	float prevY = m_ViewSizeY / 2;
	while (currentScreen < numScreens)
	{
		sf::Vector2f pos((float)rand() / (float)(RAND_MAX) * (m_ViewSizeX * 15 / 16) + (m_ViewSizeX / 16), (float)rand() / (float)(RAND_MAX)*maxYDistance + (minYDistance + prevY));
		AddEnemy(type, pos);
		if (floor(prevY / m_ViewSizeY) < floor(pos.y / m_ViewSizeY))
		{
			currentScreen++;
		}
		prevY = pos.y;
	}
}

void EnemyManager::Update(float tf)
{
	for (int i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->Update(tf);
	}
}

void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_Enemies.size(); i++)
	{
		target.draw(*m_Enemies[i]);
	}
}