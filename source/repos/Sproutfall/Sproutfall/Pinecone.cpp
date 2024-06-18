#include "Pinecone.h"

Pinecone::Pinecone(sf::Texture* texture, sf::Texture* bulletTexture, Player* player, sf::Shader* whiteShader)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);
	m_Sprite->setScale(2, 2);
	m_bulletTexture = bulletTexture;
	m_Player = player;
	m_bulletManager = make_unique<EnemyBulletManager>(bulletTexture);
	m_whiteShader = whiteShader;

	m_hasProjectiles = true;

	//CONFIGURE ANIMATIONS
	m_Sprite->setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Pinecone::Update(float tf)
{
	if (abs(m_Player->getPosition().y - getPosition().y) < 1000)
	{
		if (m_currentFireTime <= 0)
		{
			m_bulletManager->spawnVolley(m_Player->getPosition() - getPosition(), getPosition());
		}
		m_currentFireTime += tf;
	}
	m_bulletManager->Update(tf);
	if (m_currentFireTime > 0)
	{
		m_currentFireTime += tf;
		if (m_currentFireTime > m_fireTimeCooldown)
		{
			m_currentFireTime = 0;
		}
	}
}

bool Pinecone::checkProjectiles()
{
	return m_bulletManager->checkCollisions(m_Player);
}

void Pinecone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_bulletManager);
}
