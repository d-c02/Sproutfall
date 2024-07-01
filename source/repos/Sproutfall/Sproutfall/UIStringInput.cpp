#include "UIStringInput.h"

UIStringInput::UIStringInput(sf::Texture* atlasTexture, string* modifiedString, int maxLength, sf::Vector2f position)
{
	m_Length = maxLength;
	m_atlasTexture = atlasTexture;
	m_ModifiedString = modifiedString;
	for (int i = 0; i < m_Length; i++)
	{
		m_Sprites.push_back(make_unique<sf::Sprite>());
		m_Sprites[i]->setTexture(*m_atlasTexture);
		m_Sprites[i]->setOrigin(16, 16);
		m_Sprites[i]->setScale(2, 2);
		m_Sprites[i]->setTextureRect(m_DashCoords);
	}
	m_Position = position;
	setPosition(sf::Vector2f(0, 0));
}

void UIStringInput::setPosition(sf::Vector2f pos)
{
	for (int i = 0; i < m_Length; i++)
	{
		m_Sprites[i]->setPosition(m_Position.x + pos.x + (i * 64), m_Position.y + pos.y);
	}
}

void UIStringInput::handleInput(sf::Event* event)
{
	if (event->type == sf::Event::TextEntered)
	{
		char input = event->text.unicode;
		if (isalpha(input))
		{
			m_Sprites[m_CurMod]->setTextureRect(m_Letters[toupper(input) - 'A']);
			
			if (m_CurMod < m_Length - 1)
			{
				m_ModifiedString->push_back(toupper(input));
				m_CurMod++;
				m_DelMod = m_CurMod - 1;
			}
			else
			{
				if (m_ModifiedString->size() < m_Length)
				{
					m_ModifiedString->push_back(toupper(input));
				}
				else
				{
					m_ModifiedString->pop_back();
					m_ModifiedString->push_back(toupper(input));
				}


				m_DelMod = m_Length - 1;
			}
		}
		else if (input == '\b')
		{
			if (m_ModifiedString->size() > 0)
			{
				m_ModifiedString->pop_back();
			}
			if (m_DelMod == m_Length - 1)
			{
				m_Sprites[m_DelMod]->setTextureRect(m_DashCoords);
				m_DelMod--;
			}
			else if (m_DelMod > 0)
			{
				m_CurMod--;
				m_Sprites[m_DelMod]->setTextureRect(m_DashCoords);
				m_DelMod--;
			}
			else
			{
				if (m_CurMod > 0)
				{
					m_CurMod = 0;
				}
				m_Sprites[m_DelMod]->setTextureRect(m_DashCoords);
			}
		}
		cout << *m_ModifiedString << endl;
	}
}

void UIStringInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//cout << m_Position.x << ", " << m_Position.y << endl;
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		//cout << i << ": " << m_Sprites[i]->getPosition().x << ", " << m_Sprites[i]->getPosition().y << endl;
		target.draw(*m_Sprites[i]);
	}
}
