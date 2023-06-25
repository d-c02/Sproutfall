#include "Bullet.h"
Bullet::Bullet(sf::Texture* Texture, sf::Vector2f direction, float degrees, sf::Vector2f initialPosition)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = Texture;
	m_Sprite->setTexture(*m_Texture);
	m_directionX = direction.x;
	m_directionY = direction.y;
	m_Sprite->setPosition(initialPosition);
	m_Sprite->setRotation(degrees);
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	configureAnimations();
}

Bullet::~Bullet()
{

}

void Bullet::Update(float tf)
{
	m_Sprite->move(m_directionX * m_speed * tf, m_directionY * m_speed * tf);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}

void Bullet::configureAnimations()
{
	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 13, 6));
	m_AnimationManager->addState(neutral, frameVector, true, 1.0f);
	frameVector.clear();

	//Colliding state
	frameVector.push_back(sf::IntRect(50, 0, 13, 18));
	frameVector.push_back(sf::IntRect(63, 0, 13, 20));
	frameVector.push_back(sf::IntRect(74, 0, 8, 26));
	m_AnimationManager->addState(colliding, frameVector, false, 1.0f);
	frameVector.clear();

	//Despawning state
	frameVector.push_back(sf::IntRect(13, 0, 9, 6));
	frameVector.push_back(sf::IntRect(22, 0, 6, 5));
	frameVector.push_back(sf::IntRect(28, 0, 6, 5));
	frameVector.push_back(sf::IntRect(34, 0, 8, 7));
	frameVector.push_back(sf::IntRect(42, 0, 8, 7));
	m_AnimationManager->addState(despawning, frameVector, false, 0.1f);
	frameVector.clear();

	m_CurrentState = neutral;
	m_AnimationManager->setState(neutral);
}

void Bullet::checkCollision(Enemy* enemy)
{
	if (m_Sprite->getGlobalBounds().intersects(enemy->getGlobalBounds()) && enemy->getHittable())
	{
		m_CurrentState = colliding;
		m_Alive = false;
	}
}

bool Bullet::canDespawn()
{
	return (m_AnimationManager->isPlaying() && m_Alive);
}