#include "BackgroundElement.h"

BackgroundElement::BackgroundElement(sf::Vector2f position, float endPosition, string texturePath, vector<sf::IntRect> frameVector, float frameDelay, float levelSize, float verticalOffset)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile(texturePath))
	{
		cout << "Animation element texture load failure";
	}
	m_Sprite->setScale(2, 2);
	m_Sprite->setPosition(position);
	m_Sprite->setTexture(*m_Texture);
	m_AnimationManager = make_unique<AnimationManager>(m_Sprite.get());
	m_AnimationManager->addState(0, frameVector, true, frameDelay);

	m_StartPosition = position;
	m_EndPositionY = endPosition;
	m_levelSize = levelSize;
	m_verticalOffset = verticalOffset;
}



void BackgroundElement::UpdatePosition(float pos)
{
	//if (m_holdFlush && m_Player->getPosition().y - 480 > m_Sprite->getPosition().y - m_holdVert)
	//{
	//	
	//	m_Sprite->setPosition(m_Sprite->getPosition().x, m_Player->getPosition().y - 480 + m_holdVert);
	//	m_Sprite->move(0, m_Player->getVelocity().y * tf);
	//}
	//else
	//{
	//	m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed);
	//}
	//m_AnimationManager->Update(tf);

	float t = (pos - m_StartPosition.y) / (m_levelSize);
	float newPos = ((m_StartPosition.y) * (1 - t)) + (m_EndPositionY * t);
	m_Sprite->setPosition(m_StartPosition.x, newPos + m_verticalOffset);

}

void BackgroundElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite.get());
}

void BackgroundElement::setScale(float x, float y)
{
	m_Sprite->setScale(x, y);
}

void BackgroundElement::setPosition(float x, float y)
{
	m_Sprite->setPosition(x, y);
}

void BackgroundElement::Update(float tf)
{
	m_AnimationManager->Update(tf);
}

