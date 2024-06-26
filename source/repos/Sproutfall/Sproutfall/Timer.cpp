#include "Timer.h"

Timer::Timer(sf::Texture* Texture, sf::Vector2f position)
{
	m_Position = position;
	m_Texture = Texture;
	for (int i = 0; i < 4; i++)
	{
		m_Sprites.push_back(make_unique<sf::Sprite>());
		m_Sprites[i]->setTexture(*m_Texture);
		m_Sprites[i]->setOrigin(32, 32);
		m_Sprites[i]->setScale(2, 2);
		if (i == 1)
		{
			m_Sprites[i]->setTextureRect(m_ColonTextureRect);
		}
		else
		{
			m_Sprites[i]->setTextureRect(m_DisplayPos[0]);
		}
	}
	setPosition(position);
}

void Timer::setPosition(sf::Vector2f pos)
{
	m_Position = pos;
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		m_Sprites[i]->setPosition(pos.x + m_horizontalLetterOffset * i + m_leftShift, pos.y);
	}
}

void Timer::Update(float tf)
{
	if (!m_timedOut)
	{
		m_SecondCounter += tf;
		if (m_SecondCounter >= 1)
		{
			while (m_SecondCounter >= 1)
			{
				m_Seconds++;
				m_SecondCounter -= 1;
			}
			if (m_Seconds >= 60)
			{
				while (m_Seconds >= 60)
				{
					m_Minutes++;
					m_Seconds -= 60;
				}
				if (m_minToSecIndex == 1 && m_Minutes >= 10)
				{
					m_minToSecIndex++;
					m_Sprites.push_back(make_unique<sf::Sprite>());
					m_Sprites[m_Sprites.size() - 1]->setOrigin(32, 32);
					m_Sprites[m_Sprites.size() - 1]->setScale(2, 2);
					m_Sprites[m_Sprites.size() - 1]->setTexture(*m_Texture);
					m_leftShift -= m_horizontalLetterOffset;
					m_Sprites[0]->setTextureRect(m_DisplayPos[1]);
					m_Sprites[m_minToSecIndex]->setTextureRect(m_ColonTextureRect);
				}

				if (m_Minutes >= 60)
				{
					while (m_Minutes >= 60)
					{
						m_Hours++;
						m_Minutes -= 60;
					}
					if (m_hourToMinIndex < 0)
					{
						m_hourToMinIndex = 1;
						m_minToSecIndex += 2;
						m_Sprites.push_back(make_unique<sf::Sprite>());
						m_Sprites[m_Sprites.size() - 1]->setOrigin(32, 32);
						m_Sprites[m_Sprites.size() - 1]->setScale(2, 2);
						m_Sprites[m_Sprites.size() - 1]->setTexture(*m_Texture);
						m_Sprites.push_back(make_unique<sf::Sprite>());
						m_Sprites[m_Sprites.size() - 1]->setOrigin(32, 32);
						m_Sprites[m_Sprites.size() - 1]->setScale(2, 2);
						m_Sprites[m_Sprites.size() - 1]->setTexture(*m_Texture);
						m_leftShift -= m_horizontalLetterOffset * 2;
						m_Sprites[0]->setTextureRect(m_DisplayPos[1]);
						m_Sprites[m_minToSecIndex]->setTextureRect(m_ColonTextureRect);
						m_Sprites[m_hourToMinIndex]->setTextureRect(m_ColonTextureRect);
					}

					if (m_Hours >= 10)
					{
						Timeout();
						return;
					}

					int hours = m_Hours;
					for (int i = m_hourToMinIndex - 1; i >= 0; i--)
					{
						int rem = hours % 10;
						m_Sprites[i]->setTextureRect(m_DisplayPos[rem]);
						hours /= 10;
					}
				}


				int mins = m_Minutes;

				for (int i = m_minToSecIndex - 1; i > m_hourToMinIndex; i--)
				{
					int rem = mins % 10;
					m_Sprites[i]->setTextureRect(m_DisplayPos[rem]);
					mins /= 10;
				}

			}

			int secs = m_Seconds;

			for (int i = m_Sprites.size() - 1; i > m_minToSecIndex; i--)
			{
				int rem = secs % 10;
				m_Sprites[i]->setTextureRect(m_DisplayPos[rem]);
				secs /= 10;
			}
		}
	}
}

void Timer::Timeout()
{
	m_timedOut = true;
	m_Sprites.clear();
	m_leftShift = 0;
	m_Hours = 10;
	m_Seconds = 0;
	m_Minutes = 0;
	for (int i = 0; i < 4; i++)
	{
		m_Sprites.push_back(make_unique<sf::Sprite>());
		m_Sprites[i]->setTexture(*m_Texture);
		m_Sprites[i]->setOrigin(32, 32);
		m_Sprites[i]->setScale(2, 2);
		m_Sprites[i]->setTextureRect(m_SLOWTextureRects[i]);
	}
	setPosition(m_Position);
}

void Timer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		target.draw(*m_Sprites[i]);
	}
}
