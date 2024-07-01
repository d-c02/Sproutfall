#include "UIHighscoreView.h"

UIHighscoreView::UIHighscoreView(sf::Texture* atlasTexture, sf::Vector2f position, HighScoreManager* highScoreManager)
{
	m_atlasTexture = atlasTexture;
	m_HighscoreManager = highScoreManager;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_Highscores[i][j] = make_unique<sf::Sprite>();
			m_Highscores[i][j]->setTexture(*m_atlasTexture);
			m_Highscores[i][j]->setOrigin(16, 16);
			m_Highscores[i][j]->setScale(2, 2);
			if (j != 3 && j != 4 && j != 6 && j != 9)
			{
				m_Highscores[i][j]->setTextureRect(m_DashCoords);
			}
			else
			{
				if (j != 4)
				{
					m_Highscores[i][j]->setTextureRect(m_ColonCoords);
				}
				else
				{
					m_Highscores[i][j]->setTextureRect(sf::IntRect(0, 0, 1, 1));
				}
			}
		}
	}
	m_UIPosition = position;
	setPosition(sf::Vector2f(0, 0));
	ParseScores();
}

void UIHighscoreView::setPosition(sf::Vector2f pos)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_Highscores[i][j]->setPosition(m_UIPosition.x + j * 64, m_UIPosition.y + i * m_verticalDist);
		}
	}
}

void UIHighscoreView::updateHighScores()
{
	for (int i = 0; i < 5; i++)
	{
		m_Highscores[i]->reset();
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_Highscores[i][j] = make_unique<sf::Sprite>();
			m_Highscores[i][j]->setTexture(*m_atlasTexture);
			m_Highscores[i][j]->setOrigin(16, 16);
			m_Highscores[i][j]->setScale(2, 2);
			if (j != 3 && j != 4 && j != 6 && j != 9)
			{
				m_Highscores[i][j]->setTextureRect(m_DashCoords);
			}
			else
			{
				if (j != 4)
				{
					m_Highscores[i][j]->setTextureRect(m_ColonCoords);
				}
				else
				{
					m_Highscores[i][j]->setTextureRect(sf::IntRect(0, 0, 1, 1));
				}
			}
		}
	}
	ParseScores();
}

void UIHighscoreView::ParseScores()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_HighscoreManager->getPopulated(i))
		{
			string name = m_HighscoreManager->getName(i);
			for (int j = 0; j < 3; j++)
			{
				if (j < name.size())
				{
					m_Highscores[i][j]->setTextureRect(m_Letters[toupper(name[j]) - 'A']);
				}
				else
				{
					m_Highscores[i][j]->setTextureRect(m_DashCoords);
				}
			}
			m_Highscores[i][3]->setTextureRect(m_ColonCoords);
			m_Highscores[i][4]->setTextureRect(sf::IntRect(0, 0, 1, 1));
			int hours = m_HighscoreManager->getMins(i) / 60;
			if (hours >= 10)
			{
				m_Highscores[i][5]->setTextureRect(m_Letters['S' - 'A']);
				m_Highscores[i][6]->setTextureRect(m_Letters['L' - 'A']);
				m_Highscores[i][7]->setTextureRect(m_Letters['O' - 'A']);
				m_Highscores[i][8]->setTextureRect(m_Letters['W' - 'A']);
				for (int j = 9; j < 12; j++)
				{
					m_Highscores[i][j]->setTextureRect(sf::IntRect(0, 0, 1, 1));
				}
			}
			else
			{
				m_Highscores[i][5]->setTextureRect(m_Numbers[hours]);
				m_Highscores[i][6]->setTextureRect(m_ColonCoords);
				int mins = m_HighscoreManager->getMins(i);
				mins -= (hours * 60);
				m_Highscores[i][7]->setTextureRect(m_Numbers[mins / 10]);
				m_Highscores[i][8]->setTextureRect(m_Numbers[mins % 10]);
				m_Highscores[i][9]->setTextureRect(m_ColonCoords);
				int secs = m_HighscoreManager->getSecs(i);
				m_Highscores[i][10]->setTextureRect(m_Numbers[secs / 10]);
				m_Highscores[i][11]->setTextureRect(m_Numbers[secs % 10]);

			}
		}
	}
}

void UIHighscoreView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			target.draw(*m_Highscores[i][j]);
		}
	}
}
