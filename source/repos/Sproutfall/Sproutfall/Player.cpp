#pragma once
#include "SFML_Lib.h"
#include "Player.h"

Player::Player(sf::RenderWindow* window)
{
	m_Sprite = new sf::Sprite();
	m_Texture = new sf::Texture();
	if (!m_Texture->loadFromFile("Textures/Player.png"))
	{
		cout << "Player texture load failure";
	}
	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setOrigin(8, 8);
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(200, ground);
	m_Reticle = new Reticle(m_Sprite, window);
}

Player::~Player()
{
	delete(m_Sprite);
	delete(m_Texture);
	delete(m_Reticle);
}

void Player::Shoot()
{
	if (m_bullets > 0)
	{
		cout << m_bullets << " bullets\n";
		float tmpVelocityX = (m_Reticle->getPosition().x - getPosition().x) * m_recoil;
		float tmpVelocityY = (m_Reticle->getPosition().y - getPosition().y) * m_recoil;
		m_VelocityX += tmpVelocityX;
		m_VelocityY += tmpVelocityY;
		m_bullets--;
	}
	else
	{
		//Play empty gun sound
	}
	m_reloadProgress = 0;
}

void Player::Update(float tf)
{
	if (m_Sprite->getPosition().y > ground)
	{
		m_Sprite->setPosition(m_Sprite->getPosition().x, ground);
		m_VelocityY = 0;
	}
	if (m_Sprite->getPosition().x > 1180)
	{
		m_Sprite->setPosition(1180, m_Sprite->getPosition().y);
		m_VelocityX = 0;
	}
	else if (m_Sprite->getPosition().x < 100)
	{
		m_Sprite->setPosition(100, m_Sprite->getPosition().y);
		m_VelocityX = 0;
	}
	if (m_Sprite->getPosition().y < ground)
	{
		if (m_VelocityY < m_terminalVelocity)
		{
			m_VelocityY += m_gravity * tf;
		}
		else if (m_VelocityX > m_terminalVelocity)
		{
			m_VelocityY += m_airResistance * tf;
		}
	} 
	if (m_VelocityX > 0)
	{
		m_VelocityX += m_airResistanceX * tf;
	}
	m_Sprite->move(m_VelocityX * tf, m_VelocityY * tf);
	m_Reticle->Update(tf);
	if (m_bullets < m_bulletsMax)
	{
		m_reloadProgress += tf;
	}
	if (m_reloadProgress >= m_reloadDelay)
	{
		m_reloadProgress = 0;
		m_bullets++;
	}
}

void Player::handleInput(sf::Event* event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->key.code == sf::Mouse::Left)
		{
			Shoot();
		}
	}
}

sf::FloatRect Player::getGlobalBounds()
{
	return m_Sprite->getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_Reticle);
}