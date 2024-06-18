#include "EnemyBulletManager.h"

EnemyBulletManager::EnemyBulletManager(sf::Texture* bulletTexture)
{
	m_bulletTexture = bulletTexture;
}

void EnemyBulletManager::removeBullet(int index)
{
	m_bulletVector.erase(m_bulletVector.begin() + index);
}

void EnemyBulletManager::spawnVolley(sf::Vector2f direction, sf::Vector2f initialPos)
{
	for (int i = 0; i < m_volleySize; i++)
	{
		float scalingFactor = 1;
		if (direction.x != 0)
		{
			float rads = atan(direction.y / direction.x);
			float rotationMod = 0;
			if (direction.x < 0)
			{
				scalingFactor = scalingFactor * -1;
				rotationMod = 180;
			}
			float randomFloat = (float)(rand()) / (float)(RAND_MAX);
			if (rand() % 2 == 0)
			{
				rads += randomFloat * m_spread;
			}
			else
			{
				rads -= randomFloat * m_spread;
			}
			double tmpx = (scalingFactor * cos(rads));
			double tmpy = (scalingFactor * sin(rads));
			float rotation = rotationMod + (rads * (180.0 / 3.141592653589793238463));
			m_bulletVector.push_back(make_unique<EnemyBullet>(m_bulletTexture, sf::Vector2f(tmpx, tmpy), rotation, initialPos, ShotgunPellet, m_shotgunPelletSpeed + (((float)rand() - (RAND_MAX / 2)) / (float)RAND_MAX / 2) * m_randomInitialPelletSpeed));
		}
		else
		{
			m_bulletVector.push_back(make_unique<EnemyBullet>(m_bulletTexture, sf::Vector2f(0, direction.y), -90, initialPos, ShotgunPellet, m_shotgunPelletSpeed + (((float)rand() - (RAND_MAX / 2)) / (float)RAND_MAX / 2) * m_randomInitialPelletSpeed));
		}
	}
}

void EnemyBulletManager::spawnPineconeExplosion(sf::Vector2f initialPos)
{
}

void EnemyBulletManager::spawnBullet(sf::Vector2f direction, sf::Vector2f initialPos)
{
}

void EnemyBulletManager::Update(float tf)
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		m_bulletVector[i]->Update(tf);
		if (m_bulletVector[i]->canDespawn())
		{
			removeBullet(i);
		}
	}
}

bool EnemyBulletManager::checkCollisions(Player* player)
{
	sf::CircleShape* hitbox = dynamic_cast<sf::CircleShape*>(player->getHitbox());
	bool collided = false;
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		if (m_bulletVector[i]->checkBulletCollision(hitbox))
		{
			collided = true;
		}
	}
	return collided;
}

void EnemyBulletManager::clearBullets()
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		m_bulletVector[i]->killBullet();
	}
}

void EnemyBulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		target.draw(*m_bulletVector[i]);
	}
}


