#include "UIButton.h"

UIButton::UIButton(string TexturePath, bool* switchBool, sf::Vector2f position, sf::IntRect unclickedTextureCoords, sf::IntRect clickedTextureCoords, sf::IntRect releasedTextureCoords, sf::RenderWindow* window, sf::Vector2f hitboxOffset)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();

	if (!m_Texture->loadFromFile(TexturePath))
	{
		cout << "Button texture load failure\n";
	}

	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);

	m_ClickedTextureCoords = clickedTextureCoords;
	m_UnclickedTextureCoords = unclickedTextureCoords;
	m_ReleasedTextureCoords = releasedTextureCoords;

	m_Sprite->setTextureRect(m_UnclickedTextureCoords);

	m_UIPosition = position;
	m_Sprite->setPosition(m_UIPosition);

	m_Hitbox = m_Sprite->getGlobalBounds();
	m_Hitbox.left = position.x + m_HitboxOffset.x;
	m_Hitbox.top = position.y + m_HitboxOffset.y;

	m_Switch = switchBool;

	m_renderWindow = window;
}

void UIButton::setPosition(sf::Vector2f pos)
{
	m_Sprite->setPosition(m_UIPosition + pos);
	m_Hitbox.left = (m_UIPosition + pos).x + m_HitboxOffset.x;
	m_Hitbox.top = (m_UIPosition + pos).y + m_HitboxOffset.y;
}

void UIButton::Update(float tf)
{

}

void UIButton::checkClick(sf::Vector2f pos)
{
	if (pos.x > m_Hitbox.left && pos.x < m_Hitbox.left + m_Hitbox.width && pos.y > m_Hitbox.top && pos.y < m_Hitbox.top + m_Hitbox.height)
	{
		m_Sprite->setTextureRect(m_ClickedTextureCoords);
		m_clicked = true;
	}
}

void UIButton::checkClickRelease(sf::Vector2f pos)
{
	if (m_clicked)
	{
		if (pos.x > m_Hitbox.left && pos.x < m_Hitbox.left + m_Hitbox.width && pos.y > m_Hitbox.top && pos.y < m_Hitbox.top + m_Hitbox.height)
		{
			if (*m_Switch)
			{
				*m_Switch = false;
				m_Sprite->setTextureRect(m_UnclickedTextureCoords);
			}
			else
			{
				*m_Switch = true;
				m_Sprite->setTextureRect(m_ReleasedTextureCoords);
			}
		}
		else
		{
			if (*m_Switch)
			{
				m_Sprite->setTextureRect(m_ReleasedTextureCoords);
			}
			else
			{
				m_Sprite->setTextureRect(m_UnclickedTextureCoords);
			}
		}
		m_clicked = false;
	}
}

void UIButton::handleInput(sf::Event* event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		checkClick(static_cast<sf::Vector2f>(m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow))));
	}
	else if (event->type == sf::Event::MouseButtonReleased)
	{
		checkClickRelease(static_cast<sf::Vector2f>(m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow))));
	}
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(m_Hitbox.width, m_Hitbox.height));
	//rect.setPosition(sf::Vector2f(m_Hitbox.left + m_HitboxOffset.x, m_Hitbox.top + m_HitboxOffset.y));
	//rect.setFillColor(sf::Color::White);
	target.draw(*m_Sprite);
	//target.draw(rect);
}
