#include "BackgroundLayer.h"
BackgroundLayer::BackgroundLayer(Player* player, float parallaxSpeed, float viewSizeX, float viewSizeY, sf::Texture* texture)
{
	m_Sprite = new sf::Sprite();
	m_Upper = new sf::Sprite();
	m_Lower = new sf::Sprite();
	m_Texture = texture;
	m_Sprite->setTexture(*m_Texture);
	m_Upper->setTexture(*m_Texture);
	m_Lower->setTexture(*m_Texture);
	m_ViewSizeX = viewSizeX;
	m_ViewSizeY = viewSizeY;
	m_Player = player;
	m_ParallaxSpeed = parallaxSpeed;
}
BackgroundLayer::~BackgroundLayer()
{
	delete(m_Sprite);
	delete(m_Texture);
	delete(m_Upper);
	delete(m_Lower);
}
void BackgroundLayer::Update(float tf)
{
	float top = round(m_Player->getPosition().y - (m_ViewSizeY / 2));
	float bottom = top + m_ViewSizeY;
	float spritePosY = m_Sprite->getPosition().y;
	if (top < spritePosY)
	{
		spritePosY -= m_ViewSizeY;
	}
	if (bottom > spritePosY + m_ViewSizeY)
	{
		spritePosY += m_ViewSizeY;
	}
	m_Sprite->setPosition(0, spritePosY);
	m_Upper->setPosition(0, spritePosY - m_ViewSizeY);
	m_Lower->setPosition(0, spritePosY + m_ViewSizeY);
	m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed);
}

void BackgroundLayer::setScale(float x, float y)
{
	m_Sprite->setScale(x, y);
	m_Upper->setScale(x, y);
	m_Lower->setScale(x, y);
}

void BackgroundLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_Upper);
	target.draw(*m_Lower);
}