#include "UICounter.h"

UICounter::UICounter(string TexturePath, int* countVal, int maxVal, sf::IntRect ActiveTextureCoords, sf::IntRect InactiveTextureCoords, sf::Vector2f position)
{
	m_ActiveTextureCoords = ActiveTextureCoords;
	m_InactiveTextureCoords = InactiveTextureCoords;
	m_Texture = make_unique<sf::Texture>();
	if (!m_Texture->loadFromFile(TexturePath))
	{
		cout << TexturePath << " load failure" << endl;
	}
	for (int i = 0; i < maxVal; i++) {
		m_CounterSprites.push_back(make_unique<sf::Sprite>());
		m_CounterSprites[i]->setTexture(*m_Texture);
		m_CounterSprites[i]->setTextureRect(m_ActiveTextureCoords);
		m_CounterSprites[i]->setOrigin(m_ActiveTextureCoords.width / 2, m_ActiveTextureCoords.height / 2);
		m_CounterSprites[i]->setScale(2, 2);
		m_CounterSprites[i]->setPosition(position.x + i * m_ActiveTextureCoords.width, position.y);
	}

	m_countVal = countVal;
	m_maxVal = maxVal;
	m_prevCountVal = *m_countVal;
}

void UICounter::setPosition(sf::Vector2f pos)
{
	for (int i = 0; i < m_maxVal; i++) {
		m_CounterSprites[i]->setPosition(pos.x + i * m_ActiveTextureCoords.width, pos.y);
	}
}

void UICounter::Update(float tf)
{
	if (m_prevCountVal != *m_countVal)
	{
		for (int i = 0; i < m_CounterSprites.size(); i++)
		{
			if (i < *m_countVal)
			{
				m_CounterSprites[i]->setTextureRect(m_ActiveTextureCoords);
			}
			else
			{
				m_CounterSprites[i]->setTextureRect(m_InactiveTextureCoords);
			}
		}
		m_prevCountVal = *m_countVal;
	}
}

void UICounter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_CounterSprites.size(); i++)
	{
		target.draw(*m_CounterSprites[i]);
	}
}
