#pragma once
#include "SFML_Lib.h"
#include "Player.h"

Player::Player(sf::RenderWindow* window)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile("Textures/Player/player.png"))
	{
		cout << "Player texture load failure";
	}
	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setOrigin(4, 11);
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(m_initialPositionX, m_initialPositionY);
	m_Shotgun = make_unique<Shotgun>(m_Sprite.get(), window);
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

	setHittable(true);
	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(3.5);
	circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_RenderWindow = window;
	m_Alive = true;
	m_ShellManager = make_unique<ShellManager>();

	m_outlineShader = make_unique<sf::Shader>();
	if (!m_outlineShader->loadFromMemory(m_outlineShaderCode, sf::Shader::Fragment))
	{
		cout << "Failure to load outline shader" << endl;
	}
	m_outlineShader->setUniform("texture", sf::Shader::CurrentTexture);
	m_outlineShader->setUniform("color", sf::Glsl::Vec4(1, 1, 1, 0));
}

Player::~Player()
{

}

void Player::Shoot()
{
	if (m_bullets > 0)
	{
		float diffx = m_Shotgun->getPosition().x - getPosition().x;
		float diffy = m_Shotgun->getPosition().y - getPosition().y;
		float dist = sqrt(powf(diffx, 2) + powf(diffy, 2));
		float tmpVelocityX = diffx/dist * m_recoil;
		float tmpVelocityY = diffy/dist * m_recoil;
		m_VelocityX += tmpVelocityX;
		m_VelocityY += tmpVelocityY;
		m_bullets--;
		m_reloadProgress = 0;

		//Bullet launching
		float bulletDirX = m_Shotgun->getPosition().x - getPosition().x;
		float bulletDirY = m_Shotgun->getPosition().y - getPosition().y;
		float length = sqrt(bulletDirX * bulletDirX) + sqrt(bulletDirY * bulletDirY);
		sf::Vector2f bulletDirection = sf::Vector2f(bulletDirX / length, bulletDirY / length);
		m_bulletManager->spawnVolley(bulletDirection, m_Shotgun->getPosition());
		m_ShotgunShootSounds[rand() % m_ShotgunShootSounds.size()]->play();

		sf::Vector2f dif = (m_Shotgun->getPosition() - getPosition());
		dif = dif / sqrtf(dif.x * dif.x + dif.y * dif.y);
		dif.x = -1 * dif.x;
		dif.y = -1 * abs(dif.y);
		m_ShellManager->CreateShell(m_Sprite->getPosition(),  dif);

		m_ScreenShake = true;

		m_Shotgun->Shoot();
	}
	else
	{
		//Play empty gun sound
	}
}

void Player::Update(float tf)
{
	if (m_Health <= 0)
	{
		m_Alive = false;
	}
	if (m_Sprite->getPosition().x > 1280)
	{
		m_Sprite->setPosition(0, m_Sprite->getPosition().y);
		//m_VelocityX = 0;
	}
	else if (m_Sprite->getPosition().x < 0)
	{
		m_Sprite->setPosition(1280, m_Sprite->getPosition().y);
		//m_VelocityX = 0;
	}
	if (m_Sprite->getPosition().y < -2000)
	{
		m_Sprite->setPosition(m_Sprite->getPosition().x, -2000);
		m_VelocityY = 0;
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
	m_Shotgun->Update(tf);
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
	m_AnimationManager->Update(tf);
	if (m_CurrentState != hurt && m_CurrentState != dead)
	{
		if (m_VelocityX < neutralThreshold && m_VelocityY < neutralThreshold)
		{
			m_AnimationManager->setState(neutral);
			m_CurrentState = neutral;
		}
		else if (m_VelocityX < fallingThreshold && m_VelocityY < fallingThreshold)
		{
			m_AnimationManager->setState(falling);
			m_CurrentState = falling;
		}
		else
		{
			m_AnimationManager->setState(falling_fast);
			m_CurrentState = falling_fast;
		}
	}
	else
	{
		if (m_CurrentState == hurt)
		{
			if (!m_AnimationManager->isPlaying())
			{
				m_AnimationManager->setState(neutral);
				m_CurrentState = neutral;
				setHittable(true);
			}
		}
	}
	setDirection();
	m_Hitbox->setPosition(m_Sprite->getPosition());
	m_ShellManager->Update(tf);
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
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			Shoot();
		}
	}

}

bool Player::getStatus() {
	return m_Alive;
}

sf::FloatRect Player::getGlobalBounds()
{
	return m_Sprite->getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_ShellManager);
	target.draw(*m_Sprite, m_outlineShader.get());
	target.draw(*m_Shotgun);
	target.draw(*m_bulletManager);
	//target.draw(*m_Hitbox);
}

void Player::configureAnimations()
{
	m_AnimationManager =  make_unique<AnimationManager>(m_Sprite.get());
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 8, 15));
	frameVector.push_back(sf::IntRect(8, 0, 8, 15));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Falling state
	frameVector.push_back(sf::IntRect(16, 0, 8, 15));
	frameVector.push_back(sf::IntRect(24, 0, 8, 15));
	m_AnimationManager->addState(falling, frameVector, true, 0.2f);
	frameVector.clear();

	//Falling fast state
	frameVector.push_back(sf::IntRect(32, 0, 8, 15));
	frameVector.push_back(sf::IntRect(40, 0, 8, 15));
	m_AnimationManager->addState(falling_fast, frameVector, true, 0.01f);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(48, 0, 8, 15));
	frameVector.push_back(sf::IntRect(56, 0, 8, 15));
	frameVector.push_back(sf::IntRect(48, 0, 8, 15));
	frameVector.push_back(sf::IntRect(56, 0, 8, 15));
	frameVector.push_back(sf::IntRect(48, 0, 8, 15));
	frameVector.push_back(sf::IntRect(56, 0, 8, 15));
	frameVector.push_back(sf::IntRect(48, 0, 8, 15));
	frameVector.push_back(sf::IntRect(56, 0, 8, 15));
	m_AnimationManager->addState(hurt, frameVector, false, 0.1f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);
}

void Player::CheckCollisions(Enemy* enemy)
{
	sf::CircleShape* playerHitbox = dynamic_cast<sf::CircleShape*>(m_Hitbox.get());
	if (m_IsHittable && enemy->getHittable())
	{
		if (HitboxIsCircular(enemy->getHitbox()))
		{
			sf::CircleShape* enemyHitbox = dynamic_cast<sf::CircleShape*>(enemy->getHitbox());
			if (calculateCollision(playerHitbox, enemyHitbox))
			{
				if (m_Health > 0)
				{
					m_CurrentState = hurt;
					m_VelocityX = m_VelocityX * m_CollisionSlowdown;
					m_VelocityY = m_VelocityY * m_CollisionSlowdown;
					m_AnimationManager->setState(hurt);
					m_Health--;
				}
				else
				{
					m_CurrentState = dead;
					m_VelocityX = 0;
					m_VelocityY = 0;
				}
				setHittable(false);
			}
		}
		else
		{
			sf::RectangleShape* enemyHitbox = dynamic_cast<sf::RectangleShape*>(enemy->getHitbox());
			if (calculateCollision(playerHitbox, enemyHitbox))
			{
				enemyHitbox->setFillColor(sf::Color(0x00ff00aa));
				m_CurrentState = hurt;
				m_VelocityX = m_VelocityX * m_CollisionSlowdown;
				m_VelocityY = m_VelocityY * m_CollisionSlowdown;
				m_AnimationManager->setState(hurt);
				m_Health--;
				setHittable(false);
			}
		}
	}
	if (enemy->getShootable())
	{
		m_bulletManager->checkCollisions(enemy);
	}

	if (enemy->hasProjectiles())
	{
		enemy->checkProjectiles();
	}
}
void Player::Die()
{

}

sf::Vector2f Player::getPosition()
{
	return m_Sprite->getPosition();
}

void Player::SetShellColor(sf::Color color)
{
	m_ShellManager->SetColor(color);
}

void Player::SetShellGravity(float gravity)
{
	m_ShellManager->SetGravity(gravity);
}

bool Player::IsScreenShaking()
{
	bool returnVal = false;
	returnVal = m_ScreenShake;
	if (m_ScreenShake)
	{
		m_ScreenShake = false;
	}
	return returnVal;
}

void Player::SetFallingParams(float gravity, float terminalVelocity)
{
	m_gravity = gravity;
	m_terminalVelocity = terminalVelocity;
}

void Player::SetOutlineColor(sf::Glsl::Vec4 color)
{
	m_outlineShader->setUniform("color", color);
}

void Player::SetWindow(sf::RenderWindow* window)
{
	m_RenderWindow = window;
}

bool Player::isHurt()
{
	return m_CurrentState == hurt;
}
