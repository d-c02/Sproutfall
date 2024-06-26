#include "Timer.h"

Timer::Timer(sf::Texture* Texture, sf::Vector2f position)
{
	m_Texture = Texture;
	for (int i = 0; i < 4; i++)
	{
		if (i == 1)
		{
			m_Sprites[i]->setTextureRect(m_ColonTextureRect);
		}
		else
		{
			m_Sprites[i]->setTextureRect(m_DisplayPos[0]);
		}
		m_Sprites.push_back(make_unique<sf::Sprite>());
		m_Sprites[i]->setTexture(*m_Texture);
		m_Sprites[i]->setOrigin(32, 32);
		m_Sprites[i]->setScale(2, 2);
	}
	setPosition(position);
}

void Timer::setPosition(sf::Vector2f pos)
{
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		m_Sprites[i]->setPosition(pos.x + m_horizontalLetterOffset * i, pos.y);
	}
}

void Timer::Update(float tf)
{
	m_SecondCounter += tf;
	if (m_SecondCounter > 1)
	{
		m_Seconds++;
		m_SecondCounter -= 1;
		if (m_Seconds >= 60)
		{
			m_Minutes++;
			if (m_Minutes > pow(10, m_numMinLetters))
			{
				m_numMinLetters++;
				m_Sprites.push_back(make_unique<sf::Sprite>());
				m_Sprites[m_Sprites.size() - 1]->setOrigin(32, 32);
				m_Sprites[m_Sprites.size() - 1]->setScale(2, 2);
				m_Sprites[m_Sprites.size() - 1]->setTexture(*m_Texture);
				setPosition(sf::Vector2f(m_Sprites[0]->getPosition().x - m_horizontalLetterOffset, m_Sprites[0]->getPosition().y));
			}
		}

		m_Sprites[m_numMinLetters]->setTextureRect(m_ColonTextureRect);
	}
}

void Timer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		target.draw(*m_Sprites[i]);
	}
}
