#pragma once
#include "SFML_Lib.h"
#include "UISlider.h"

UISlider::UISlider(sf::Vector2f position, float* sliderVal, bool* buttonHeld, string barTexturePath, string sliderNubTexturePath, float minVal, float maxVal, sf::RenderWindow* window, sf::Vector2f hitboxOffset)
{
	m_sliderVal = sliderVal;
	m_buttonHeld = buttonHeld;

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

	m_UIPosition = position;
	m_Sprite->setPosition(m_UIPosition);

	m_HitboxOffset = hitboxOffset;
	m_Hitbox = m_Sprite->getGlobalBounds();
	m_Hitbox.left = position.x + m_HitboxOffset.x;
	m_Hitbox.top = position.y + m_HitboxOffset.y;

	m_NubSprite->setPosition(sf::Vector2f(((*m_sliderVal - minVal) / maxVal) * m_Hitbox.width + m_Hitbox.left, position.y - (m_NubTexture->getSize().y - m_Texture->getSize().y)));

	m_minVal = minVal;
	m_maxVal = maxVal;

	m_renderWindow = window;
}

void UISlider::setPosition(sf::Vector2f pos)
{
	m_Sprite->setPosition(m_UIPosition + pos);
	m_NubSprite->setPosition(sf::Vector2f(((*m_sliderVal - m_minVal) / m_maxVal) * m_Hitbox.width + m_Hitbox.left, m_Sprite->getPosition().y - (m_NubTexture->getSize().y - m_Texture->getSize().y)));
	m_Hitbox.left = (m_UIPosition + pos).x + m_HitboxOffset.x;
	m_Hitbox.top = (m_UIPosition + pos).y + m_HitboxOffset.y;
}

void UISlider::Update(float tf)
{
	
}

void UISlider::checkClick(sf::Vector2f pos)
{
	if (pos.x > m_Hitbox.left && pos.x < m_Hitbox.left + m_Hitbox.width && pos.y > m_Hitbox.top && pos.y < m_Hitbox.top + m_Hitbox.height)
	{
		*m_buttonHeld = true;
	}
}

void UISlider::checkClickRelease(sf::Vector2f pos)
{
	*m_buttonHeld = false;
}

void UISlider::handleInput(sf::Event* event)
{
	sf::Vector2f pos = static_cast<sf::Vector2f>(m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow)));

	if (event->type == sf::Event::MouseButtonPressed)
	{
		checkClick(pos);
	}
	else if (event->type == sf::Event::MouseButtonReleased)
	{
		checkClickRelease(pos);
	}

	if (*m_buttonHeld)
	{

		if (pos.x > m_Hitbox.left + m_Hitbox.width)
		{
			pos.x = m_Hitbox.left + m_Hitbox.width;
		}

		else if (pos.x < m_Hitbox.left) {
			pos.x = m_Hitbox.left;
		}

		m_NubSprite->setPosition(pos.x, m_NubSprite->getPosition().y);

		float t = (pos.x - m_Hitbox.left) / (m_Hitbox.width);

		*m_sliderVal = (1.0f - t) * m_minVal + t * m_maxVal;
	}
}

void UISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
	target.draw(*m_NubSprite);
}
