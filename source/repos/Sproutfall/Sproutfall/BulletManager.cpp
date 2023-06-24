#include "BulletManager.h"
BulletManager::BulletManager()
{
	m_bulletTexture = new sf::Texture();
	if (!m_bulletTexture->loadFromFile("Textures/Bullet.png"))
	{
		cout << "Bullet texture load failure\n";
	}
}
BulletManager::~BulletManager()
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		delete(m_bulletVector[i]);
	}
	delete(m_bulletTexture);
}
void BulletManager::pushBullet(Bullet* bullet)
{
	m_bulletVector.push_back(bullet);
}
void BulletManager::removeBullet(int index)
{
	delete(m_bulletVector[index]);
	m_bulletVector.erase(m_bulletVector.begin() + index);
}
void BulletManager::spawnVolley(sf::Vector2f direction, sf::Vector2f initialPos)
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
			float randomFloat = (float) (rand()) / (float)(RAND_MAX);
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
			pushBullet(new Bullet(m_bulletTexture, sf::Vector2f(tmpx, tmpy), rotation, initialPos));
		}
		else
		{
			pushBullet(new Bullet(m_bulletTexture, sf::Vector2f(0, direction.y), -90, initialPos));
		}
	}
}
void BulletManager::Update(float tf)
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		m_bulletVector[i]->Update(tf);
	}
}

void BulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		target.draw(*m_bulletVector[i]);
	}
}