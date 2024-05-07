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
			if (!m_EnemyTextures[b_Cloud]->loadFromFile("Textures/Enemies/sky_badcloud.png"))
			{
				cout << "Textures/Enemies/sky_badcloud.png load failure" << endl;
			}
		}
		else if (i == b_Bird)
		{
			if (!m_EnemyTextures[b_Bird]->loadFromFile("Textures/Enemies/sky_bird.png"))
			{
				cout << "Textures/Enemies/sky_bird.png load failure" << endl;
			}
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
	m_smokeTexture = make_unique<sf::Texture>();
	if (!m_smokeTexture->loadFromFile("Textures/enemySmoke.png"))
	{
		cout << "Enemy smoke texture load failure" << endl;
	}
	m_whiteShader = make_unique<sf::Shader>();
	if (!m_whiteShader->loadFromMemory(m_whiteShaderCode, sf::Shader::Fragment))
	{
		cout << "Failure to load white shader" << endl;
	}
	m_whiteShader->setUniform("texture", sf::Shader::CurrentTexture);
}
EnemyManager::~EnemyManager()
{

}
void EnemyManager::AddEnemy(int type, sf::Vector2f position)
{
	if (type == b_Asteroid)
	{
		m_Enemies.push_back(make_unique<Asteroid>(m_EnemyTextures[b_Asteroid].get(), m_Player, m_whiteShader.get()));
	}
	else if (type == b_Squid)
	{
		m_Enemies.push_back(make_unique<Squid>(m_EnemyTextures[b_Squid].get(), m_Player, m_whiteShader.get()));
	}
	else if (type == b_Cloud)
	{
		m_Enemies.push_back(make_unique<BadCloud>(m_EnemyTextures[b_Cloud].get(), m_Player));
	}
	else if (type == b_Bird)
	{
		m_Enemies.push_back(make_unique<Bird>(m_EnemyTextures[b_Bird].get(), m_Player, m_whiteShader.get()));
	}
	else if (type == b_Branch)
	{

	}
	else if (type == b_Bug)
	{

	}

	m_Enemies[m_Enemies.size() - 1]->setPosition(position);
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

void EnemyManager::removeEnemy(int index)
{
	m_Enemies.erase(m_Enemies.begin() + index);
}

void EnemyManager::removeSmoke(int index)
{
	m_SmokeSprites.erase(m_SmokeSprites.begin() + index);
	m_SmokeAnimationManagers.erase(m_SmokeAnimationManagers.begin() + index);
}
void EnemyManager::addSmoke(sf::Vector2f position)
{
	m_SmokeSprites.push_back(make_unique<sf::Sprite>());
	m_SmokeSprites[m_SmokeSprites.size() - 1]->setTexture(*m_smokeTexture);
	m_SmokeAnimationManagers.push_back(make_unique<AnimationManager>(m_SmokeSprites[m_SmokeSprites.size() - 1].get()));
	vector<sf::IntRect> frameVector;
	frameVector.push_back(sf::IntRect(0, 0, 19, 17));
	frameVector.push_back(sf::IntRect(19, 0, 19, 18));
	frameVector.push_back(sf::IntRect(38, 0, 21, 20));
	frameVector.push_back(sf::IntRect(59, 0, 26, 23));
	frameVector.push_back(sf::IntRect(85, 0, 28, 25));
	frameVector.push_back(sf::IntRect(113, 0, 30, 27));
	frameVector.push_back(sf::IntRect(143, 0, 30, 27));
	m_SmokeSprites[m_SmokeSprites.size() - 1]->setOrigin(12, 15);
	m_SmokeSprites[m_SmokeSprites.size() - 1]->setScale(2, 2);
	m_SmokeSprites[m_SmokeSprites.size() - 1]->setPosition(position);
	m_SmokeAnimationManagers[m_SmokeAnimationManagers.size() - 1]->addState(0, frameVector, false, 0.07f);
	frameVector.clear();
	m_SmokeAnimationManagers[m_SmokeAnimationManagers.size() - 1]->Play();
}

void EnemyManager::Update(float tf)
{
	for (int i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->Update(tf);
		if (!m_Enemies[i]->GetStatus())
		{
			if (m_Enemies[i]->hasSmoke())
			{
				addSmoke(m_Enemies[i]->getPosition());
				m_SmokeAnimationManagers[m_SmokeAnimationManagers.size() - 1]->Play();
			}
			removeEnemy(i);
		}
	}
	for (int i = 0; i < m_SmokeAnimationManagers.size(); i++)
	{
		m_SmokeAnimationManagers[i]->Update(tf);
		if (!m_SmokeAnimationManagers[i]->isPlaying())
		{
			removeSmoke(i);
		}
	}
	checkCollisions();
}

void EnemyManager::Clear()
{
	m_Enemies.clear();
}

void EnemyManager::checkCollisions()
{
	for (int i = 0; i < m_Enemies.size(); i++)
	{
		m_Player->CheckCollisions(m_Enemies[i].get());
	}
}
void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_Enemies.size(); i++)
	{
		
		target.draw(*m_Enemies[i]);
	}
	for (int i = 0; i < m_SmokeSprites.size(); i++)
	{
		target.draw(*m_SmokeSprites[i]);
	}
}