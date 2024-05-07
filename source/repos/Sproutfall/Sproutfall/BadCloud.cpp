#include "BadCloud.h"

BadCloud::BadCloud(sf::Texture* texture, sf::Texture* m_lightningTexture, Player* player)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);

	if (rand() % 2 == 0)
	{
		m_Sprite->setScale(2, 2);
	}
	else
	{
		m_Sprite->setScale(-2, 2);
	}

	m_lightningSprite = make_unique<sf::Sprite>();
	m_lightningSprite->setTexture(*m_lightningTexture);
	m_lightningSprite->setScale(2, 2);
	m_lightningSprite->setOrigin(16, 0);

	auto circ = make_unique<sf::CircleShape>();
	circ->setRadius(20);
	//circ->setScale(m_Sprite->getScale());
	circ->setFillColor(sf::Color(0xff0000aa));
	circ->setOrigin(circ->getGlobalBounds().left + circ->getRadius(), circ->getGlobalBounds().top + circ->getRadius());
	m_Hitbox = std::move(circ);
	m_Shootable = false;
	setHittable(true);

	m_belowCollider = sf::RectangleShape(sf::Vector2f(32 * 2, 128 * 2));
	m_belowCollider.setOrigin(m_belowCollider.getSize().x / 2, 0);
	m_belowCollider.setFillColor(sf::Color::Blue);

	m_Player = player;
	m_hasProjectiles = true;
	configureAnimations();
	m_State = neutral;
	m_hasSmoke = false;
}

void BadCloud::Update(float tf)
{
	if (m_State == attacking && !m_AnimationManager->isPlaying())
	{
		m_alive = false;
	}
	m_Hitbox->setPosition(m_Sprite->getPosition());
	m_belowCollider.setPosition(m_Sprite->getPosition().x, m_Sprite->getPosition().y + 16);
	m_lightningSprite->setPosition(m_belowCollider.getPosition());
	m_AnimationManager->Update(tf);
	m_LightningAnimationManager->Update(tf);
}


void BadCloud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_LightningAnimationManager->isPlaying())
	{
		target.draw(*m_lightningSprite);
	}
	target.draw(*m_Sprite);
	//target.draw(*m_Hitbox);
	//target.draw(m_belowCollider);
}

bool BadCloud::checkProjectiles()
{
	sf::CircleShape* playerHitbox = dynamic_cast<sf::CircleShape*>(m_Player->getHitbox());
	if (calculateCollision(&m_belowCollider, playerHitbox) && m_State == neutral)
	{
		m_State = attacking;
		m_AnimationManager->setState(attacking);
		m_LightningAnimationManager->setState(attacking);
	}

	if (m_State == attacking)
	{
		sf::FloatRect lightningBounds = m_lightningSprite->getGlobalBounds();
		sf::RectangleShape lightningCollision = sf::RectangleShape(sf::Vector2f(lightningBounds.width, lightningBounds.height));
		lightningCollision.setPosition(lightningBounds.left, lightningBounds.top);
		if (m_LightningAnimationManager->isPlaying())
		{
			if (calculateCollision(&lightningCollision, playerHitbox))
			{
				return true;
			}
		}
	}
	return false;
}

void BadCloud::configureAnimations()
{
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());

	vector<sf::IntRect> frameVector;
	//Neutral state
	frameVector.push_back(sf::IntRect(0, 0, 32, 32));
	frameVector.push_back(sf::IntRect(32, 0, 32, 32));
	m_AnimationManager->addState(neutral, frameVector, true, 0.5f);
	frameVector.clear();

	//Attacking state
	frameVector.push_back(sf::IntRect(64, 0, 32, 32));
	frameVector.push_back(sf::IntRect(96, 0, 32, 32));
	frameVector.push_back(sf::IntRect(128, 0, 32, 32));
	frameVector.push_back(sf::IntRect(160, 0, 32, 32));
	frameVector.push_back(sf::IntRect(192, 0, 32, 32));
	frameVector.push_back(sf::IntRect(224, 0, 32, 32));
	frameVector.push_back(sf::IntRect(256, 0, 32, 32));
	frameVector.push_back(sf::IntRect(288, 0, 32, 32));
	frameVector.push_back(sf::IntRect(320, 0, 32, 32));
	frameVector.push_back(sf::IntRect(352, 0, 32, 32));
	frameVector.push_back(sf::IntRect(384, 0, 32, 32));
	frameVector.push_back(sf::IntRect(416, 0, 32, 32));
	m_AnimationManager->addState(attacking, frameVector, false, 0.075f);
	frameVector.clear();

	m_AnimationManager->setState(neutral);

	m_LightningAnimationManager = make_unique<AnimationManager>(m_lightningSprite.get());

	frameVector.push_back(sf::IntRect(0, 0, 0, 0));
	m_LightningAnimationManager->addState(neutral, frameVector, false, 0.75f);
	frameVector.clear();

	frameVector.push_back(sf::IntRect(0, 0, 32, 0));
	frameVector.push_back(sf::IntRect(32, 0, 32, 15));
	frameVector.push_back(sf::IntRect(64, 0, 32, 12));
	frameVector.push_back(sf::IntRect(96, 0, 32, 10));
	frameVector.push_back(sf::IntRect(128, 0, 32, 23));
	frameVector.push_back(sf::IntRect(160, 0, 32, 102));
	frameVector.push_back(sf::IntRect(192, 0, 32, 99));
	frameVector.push_back(sf::IntRect(224, 0, 32, 79));
	frameVector.push_back(sf::IntRect(256, 0, 32, 75));
	//frameVector.push_back(sf::IntRect(288, 0, 32, 102));
	//frameVector.push_back(sf::IntRect(320, 0, 32, 102));
	//frameVector.push_back(sf::IntRect(352, 0, 32, 102));
	m_LightningAnimationManager->addState(attacking, frameVector, false, 0.075f);
	
	m_LightningAnimationManager->setState(neutral);
}