#pragma once
#include "SFML_Lib.h"
#include "UISlider.h"

UISlider::UISlider(sf::Vector2f position, float* sliderVal, string barTexturePath, string sliderNubTexturePath, float minVal, float maxVal, sf::Vector2f hitboxOffset)
{
	m_sliderVal = sliderVal;

	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile(barTexturePath))
	{
		cout << "Slider bar texture load failure" << endl;
	}

	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);
	
	m_NubSprite = make_unique<sf::Sprite>();
	m_NubTexture = make_unique<sf::Texture>();
	if (!m_NubTexture->loadFromFile(sliderNubTexturePath))
	{
		cout << "Slider nub texture load failure" << endl;
	}

	m_NubSprite->setTexture(*m_NubTexture);
	m_NubSprite->setOrigin(m_NubTexture->getSize().x / 2, 0);
	m_NubSprite->setScale(2, 2);

	m_Sprite->setPosition(position);

	m_HitboxOffset = hitboxOffset;
	m_Hitbox = m_Sprite->getGlobalBounds();
	m_Hitbox.left = position.x + m_HitboxOffset.x;
	m_Hitbox.top = position.y + m_HitboxOffset.y;

	m_NubSprite->setPosition(sf::Vector2f(((*m_sliderVal - minVal) / maxVal) * m_Hitbox.width + m_Hitbox.left, position.y - m_NubTexture->getSize().y / 4));
}

void UISlider::setPosition(sf::Vector2f pos)
{
	m_Sprite->setPosition(pos);
	m_NubSprite->setPosition(sf::Vector2f(pos.x, pos.y - m_NubTexture->getSize().y / 2));
	m_Hitbox.left = pos.x + m_HitboxOffset.x;
	m_Hitbox.top = pos.y + m_HitboxOffset.y;
}

void UISlider::Update(float tf)
{
}

void UISlider::checkClick(sf::Vector2f pos)
{
	if (pos.x > m_Hitbox.left && pos.x < m_Hitbox.left + m_Hitbox.width && pos.y > m_Hitbox.top && pos.y < m_Hitbox.top + m_Hitbox.height)
	{
		m_NubSprite->setPosition(pos.x, m_NubSprite->getPosition().y);
		
		float t = (pos.x - m_Hitbox.left) / (m_Hitbox.left + m_Hitbox.width);
		
		*m_sliderVal = (1 - t) * m_minVal + t * m_maxVal;
	}
}

void UISlider::checkClickRelease(sf::Vector2f pos)
{
}

void UISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_NubSprite);
}
