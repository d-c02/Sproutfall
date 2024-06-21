#include "Pinecone.h"

Pinecone::Pinecone(sf::Texture* texture, sf::Texture* bulletTexture, Player* player, sf::Shader* whiteShader)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);
	m_Sprite->setScale(2, 2);
	m_ExplosionTexture = texture;
	m_bulletTexture = bulletTexture;
	m_Player = player;
	m_bulletManager = make_unique<EnemyBulletManager>(bulletTexture);
	m_whiteShader = whiteShader;
	m_hasProjectiles = true;
	m_hasSmoke = false;
	m_health = 4;
	m_whiteShader = whiteShader;
	m_blinking = false;
	m_IsHittable = true;
	m_Shootable = true;

	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(7);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);

	//CONFIGURE ANIMATIONS
	configureAnimations();
}

void Pinecone::Update(float tf)
{
	if (m_health > 0)
	{
		if (m_State == neutral && m_Player->getPosition().y > getPosition().y)
		{
			m_State = falling;
		}

		else if (m_State == falling)
		{
			m_currentFallingTime += tf;

			if (m_VelocityY < m_terminalVelocity)
			{
				m_VelocityY += m_gravity * tf;
			}
			else if (m_VelocityY > m_terminalVelocity)
			{
				m_VelocityY += m_airResistance * tf;
			}

			if (m_currentFallingTime > m_maxFallingTime)
			{
				m_Shootable = false;
				m_State = exploding;
				m_animationManager->setState(exploding);
				m_VelocityX = 0;
				m_VelocityY = 0;
				m_animationManager->Play();
			}
		}

		else if (m_State == exploding)
		{
			if (!m_animationManager->isPlaying())
			{
				m_IsHittable = false;
				m_State = exploded;
				for (int i = 0; i < m_ExplosionAnimationManagers.size(); i++)
				{
					sf::Vector2f position = sf::Vector2f(getPosition().x + (m_ExplosionVFXRange * ((float)rand() / RAND_MAX)) - m_ExplosionVFXRange / 2, getPosition().y + (m_ExplosionVFXRange * ((float)rand() / RAND_MAX)) - m_ExplosionVFXRange / 2);
					m_ExplosionVFX[i]->setPosition(position);
					m_ExplosionAnimationManagers[i]->setState(1);
					m_ExplosionAnimationManagers[i]->Play();
				}
				m_bulletManager->spawnPineconeExplosion(getPosition());
			}
		}

		else if (m_State == exploded)
		{
			m_deathTime += tf;
			if (m_deathTime >= m_maxDeathTime)
			{
				m_alive = false;
			}
		}

		m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
		m_animationManager->Update(tf);

		for (int i = 0; i < m_ExplosionAnimationManagers.size(); i++)
		{
			m_ExplosionAnimationManagers[i]->Update(tf);
		}

		m_Hitbox->setPosition(getPosition());
	}
	else
	{
		m_alive = false;
	}
	m_bulletManager->Update(tf);
}

bool Pinecone::checkProjectiles()
{
	return m_bulletManager->checkCollisions(m_Player);
}

void Pinecone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_State != exploded)
	{
		if (m_alive)
		{
			target.draw(*m_Sprite);
		}
	}

	for (int i = 0; i < m_ExplosionAnimationManagers.size(); i++)
	{
		if (m_ExplosionAnimationManagers[i]->isPlaying())
		{
			target.draw(*m_ExplosionVFX[i]);
		}
	}

	target.draw(*m_bulletManager);
}

void Pinecone::configureAnimations()
{
	vector<sf::IntRect> frameVector;
	for (int i = 0; i < 3; i++)
	{
		m_ExplosionVFX.push_back(make_unique<sf::Sprite>());
		m_ExplosionVFX[i]->setTexture(*m_ExplosionTexture);
		m_ExplosionVFX[i]->setScale(2, 2);
		m_ExplosionVFX[i]->setOrigin(16, 16);

		m_ExplosionAnimationManagers.push_back(make_unique<AnimationManager>(m_ExplosionVFX[i].get()));

		frameVector.push_back(sf::IntRect(0, 0, 1, 1));
		m_ExplosionAnimationManagers[i]->addState(0, frameVector, false, 1.0f);
		frameVector.clear();

		for (int j = 0; j < i; j++)
		{
			frameVector.push_back(sf::IntRect(0, 0, 1, 1));
		}
		frameVector.push_back(sf::IntRect(96, 0, 32, 32));
		frameVector.push_back(sf::IntRect(128, 0, 32, 32));
		m_ExplosionAnimationManagers[i]->addState(1, frameVector, false, 0.05f);
		frameVector.clear();
	}

	m_animationManager = make_unique<AnimationManager>(m_Sprite.get());

	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	m_animationManager->addState(neutral, frameVector, false, 1.0f);
	
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(0, 0, 1, 1));
	m_animationManager->addState(exploding, frameVector, false, 0.1f);

	m_State = neutral;
	m_animationManager->setState(neutral);
}
