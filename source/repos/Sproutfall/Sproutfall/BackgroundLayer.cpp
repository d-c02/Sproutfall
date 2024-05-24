#include "BackgroundLayer.h"


BackgroundLayer::BackgroundLayer()
{
}

BackgroundLayer::BackgroundLayer(float StartPos, float EndPos, float viewSizeX, float viewSizeY, string texturePath, float levelSize, bool drawOthers)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Upper = make_unique<sf::Sprite>();
	m_Lower = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile(texturePath))
	{
		cout << "Could not load texture " << texturePath << endl;
	}
	m_Sprite->setTexture(*m_Texture);
	m_Upper->setTexture(*m_Texture);
	m_Lower->setTexture(*m_Texture);
	m_ViewSizeX = viewSizeX;
	m_ViewSizeY = viewSizeY;
	m_Parallax = 1;
	m_drawOthers = drawOthers;
	m_StartPosition = StartPos;
	m_TruePosition = StartPos;
	m_EndPosition = EndPos;
	m_levelSize = levelSize;
}
BackgroundLayer::~BackgroundLayer()
{


}

void BackgroundLayer::UpdatePosition(float pos)
{
	float top = round(pos - (m_ViewSizeY / 2));
	float bottom = top + m_ViewSizeY;
	float spritePosY = m_Sprite->getPosition().y;
	float textureSize = (m_Texture->getSize().y * m_Sprite->getScale().y);

	float t = (pos - m_StartPosition) / (m_levelSize);

	if (m_drawOthers)
	{
		if (top < spritePosY)
		{
			m_ParallaxShift -= textureSize;
		}
		if (bottom > spritePosY + m_ViewSizeY)
		{
			m_ParallaxShift += textureSize;
		}
	}

	m_TruePosition = ((m_StartPosition) * (1 - t)) + (m_EndPosition * t);
	//cout << parallaxPosition << endl;
	m_Sprite->setPosition(0, m_TruePosition + m_ParallaxShift);
	m_Upper->setPosition(0, - textureSize + m_TruePosition + m_ParallaxShift);
	m_Lower->setPosition(0, + textureSize + m_TruePosition + m_ParallaxShift);

	//m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
	//m_Upper->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
	//m_Lower->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
}

//void BackgroundLayer::Update(float tf)
//{
//	float top = round(m_Player->getPosition().y - (m_ViewSizeY / 2));
//	float bottom = top + m_ViewSizeY;
//	float spritePosY = m_Sprite->getPosition().y;
//	float textureSize = (m_Texture->getSize().y * m_Sprite->getScale().y);
//
//	if (m_drawOthers)
//	{
//		if (top < spritePosY)
//		{
//			spritePosY -= textureSize;
//
//		}
//		if (bottom > spritePosY + m_ViewSizeY)
//		{
//			spritePosY += textureSize;
//		}
//		m_Sprite->setPosition(0, spritePosY);
//		m_Upper->setPosition(0, spritePosY - textureSize);
//		m_Lower->setPosition(0, spritePosY + textureSize);
//	}
//
//	m_Sprite->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
//	m_Upper->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
//	m_Lower->move(0, -m_Player->getVelocity().y * tf * m_ParallaxSpeed * m_Parallax);
//
//	//prevPosition = m_Player->getPosition();
//}

void BackgroundLayer::setScale(float x, float y)
{
	m_Sprite->setScale(x, y);
	m_Upper->setScale(x, y);
	m_Lower->setScale(x, y);
}

void BackgroundLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//float top = round(m_Player->getPosition().y - (m_ViewSizeY / 2));
	//float bottom = top + m_ViewSizeY;
	target.draw(*m_Sprite);
	if (m_drawOthers)
	{
		target.draw(*m_Upper);
		target.draw(*m_Lower);
	}
}

void BackgroundLayer::setParallax(bool parallax)
{
	if (parallax)
	{
		m_Parallax = 1;
	}
	else
	{
		m_Parallax = 0;
	}
}