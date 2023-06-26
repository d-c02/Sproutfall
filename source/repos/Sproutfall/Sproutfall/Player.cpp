#pragma once
#include "SFML_Lib.h"
#include "Player.h"

Player::Player(sf::RenderWindow* window)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile("Textures/Player/PlayerSpriteSheet.png"))
	{
		cout << "Player texture load failure";
	}
	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setOrigin(2, 10);
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(m_initialPositionX, m_initialPositionY);
	m_Reticle = make_unique<Reticle>(m_Sprite.get(), window);
	m_bulletManager = make_unique<BulletManager>();
	configureAnimations();
	for (int i = 0; i < 3; i++)
	{
		m_ShotgunShootBuffers.push_back(make_unique<sf::SoundBuffer>());
		if (!m_ShotgunShootBuffers[i]->loadFromFile("Sounds/Player/shotgun_shoot" + std::to_string(i + 1) + ".ogg"))
		{
			cout << "Shotgun shoot #" << i << " load failure\n";
		}
		m_ShotgunShootSounds.push_back(make_unique<sf::Sound>());
		m_ShotgunShootSounds[i]->setBuffer(*m_ShotgunShootBuffers[i]);
	}
}

Player::~Player()
{

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
		m_reloadProgress = 0;

		//Bullet launching
		float bulletDirX =  m_Reticle->getPosition().x - getPosition().x;
		float bulletDirY = m_Reticle->getPosition().y - getPosition().y;
		float length = sqrt(bulletDirX * bulletDirX) + sqrt(bulletDirY * bulletDirY);
		sf::Vector2f bulletDirection = sf::Vector2f(bulletDirX / length, bulletDirY / length);
		m_bulletManager->spawnVolley(bulletDirection, m_Reticle->getPosition());
		m_ShotgunShootSounds[rand() % m_ShotgunShootSounds.size()]->play();
	}
	else
	{
		//Play empty gun sound
	}
}

void Player::Update(float tf)
{
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
	if (m_VelocityY < m_terminalVelocity)
	{
		m_VelocityY += m_gravity * tf;
	}
	else if (m_VelocityY > m_terminalVelocity)
	{
		m_VelocityY += m_airResistance * tf;
	}
	if (m_VelocityX > 0)
	{
		if ((m_airResistanceX * tf) > m_VelocityX)
		{
			m_VelocityX = 0;
		}
		else
			m_VelocityX += m_airResistanceX * tf;
	}
	else if (m_VelocityX < 0)
	{
		if ((m_airResistanceX * tf) < m_VelocityX)
		{
			m_VelocityX = 0;
		}
		else
			m_VelocityX -= m_airResistanceX * tf;
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
	m_bulletManager->Update(tf);
	tf = 1.5;
	m_AnimationManager->Update(tf);
	if (m_VelocityX < neutralThreshold && m_VelocityY < neutralThreshold)
	{
		m_AnimationManager->setState(neutral);
	}
	else if (m_VelocityX < fallingThreshold && m_VelocityY < fallingThreshold)
	{
		m_AnimationManager->setState(falling);
	}
	else
	{
		m_AnimationManager->setState(falling_fast);
	}
	setDirection();
}

void Player::setDirection()
{
	float rads = 3.141592653589793238463 / 2;
	if (m_VelocityX != 0)
		rads = atan(m_VelocityY / m_VelocityX);
	if (m_VelocityX < 0 && m_SpriteFlipped == false)
	{
		m_Sprite->scale(-1, 1);
		m_SpriteFlipped = true;
	}
	else if (m_SpriteFlipped == true && m_VelocityX >= 0)
	{
		m_SpriteFlipped = false;
		m_Sprite->scale(-1, 1);
	}
	float degrees = 0;
	if (m_SpriteFlipped)
		degrees = rads * (180.0 / 3.141592653589793238463) + 90;
	else
		degrees = rads * (180.0 / 3.141592653589793238463) + 270;
	if (m_VelocityX == 0 && m_VelocityY < 0)
	{
		degrees = degrees + 180;
	}
	m_Sprite->setRotation(degrees);
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
	target.draw(*m_bulletManager);
}

void Player::configureAnimations()
{
	m_AnimationManager =  make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 6, 13));
	frameVector.push_back(sf::IntRect(6, 0, 6, 13));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Falling state
	frameVector.push_back(sf::IntRect(12, 0, 6, 13));
	frameVector.push_back(sf::IntRect(18, 0, 6, 13));
	m_AnimationManager->addState(falling, frameVector, true, 0.2f);
	frameVector.clear();

	//Falling fast state
	frameVector.push_back(sf::IntRect(24, 0, 6, 13));
	frameVector.push_back(sf::IntRect(30, 0, 6, 13));
	m_AnimationManager->addState(falling_fast, frameVector, true, 0.1f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);
}

void Player::CheckCollisions(Enemy* enemy)
{
	m_bulletManager->checkCollisions(enemy);
}
void Player::Die()
{

}