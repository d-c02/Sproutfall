#pragma once
#include "SFML_Lib.h"
#include "UIElement.h"
#include "HighScoreManager.h"

class UIHighscoreView :public UIElement
{
public:
	UIHighscoreView(sf::Texture* atlasTexture, sf::Vector2f position, HighScoreManager* highScoreManager);
	void setPosition(sf::Vector2f pos) override;
private:
	void ParseScores();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	unique_ptr<sf::Sprite> m_Highscores[5][12];
	sf::Texture* m_atlasTexture;
	const float m_verticalDist = 100;

	sf::IntRect m_Letters[26] =
	{
		sf::IntRect(0, 0, 32, 32),
		sf::IntRect(32, 0, 32, 32),
		sf::IntRect(64, 0, 32, 32),
		sf::IntRect(96, 0, 32, 32),
		sf::IntRect(128, 0, 32, 32),
		sf::IntRect(160, 0, 32, 32),
		sf::IntRect(0, 32, 32, 32),
		sf::IntRect(32, 32, 32, 32),
		sf::IntRect(64, 32, 32, 32),
		sf::IntRect(96, 32, 32, 32),
		sf::IntRect(128, 32, 32, 32),
		sf::IntRect(160, 32, 32, 32),
		sf::IntRect(0, 64, 32, 32),
		sf::IntRect(32, 64, 32, 32),
		sf::IntRect(64, 64, 32, 32),
		sf::IntRect(96, 64, 32, 32),
		sf::IntRect(128, 64, 32, 32),
		sf::IntRect(160, 64, 32, 32),
		sf::IntRect(0, 96, 32, 32),
		sf::IntRect(32, 96, 32, 32),
		sf::IntRect(64, 96, 32, 32),
		sf::IntRect(96, 96, 32, 32),
		sf::IntRect(128, 96, 32, 32),
		sf::IntRect(160, 96, 32, 32),
		sf::IntRect(0, 128, 32, 32),
		sf::IntRect(32, 128, 32, 32)
	};

	sf::IntRect m_Numbers[10] =
	{
		sf::IntRect(64, 128, 32, 32),
		sf::IntRect(96, 128, 32, 32),
		sf::IntRect(128, 128, 32, 32),
		sf::IntRect(160, 128, 32, 32),
		sf::IntRect(0, 160, 32, 32),
		sf::IntRect(32, 160, 32, 32),
		sf::IntRect(64, 160, 32, 32),
		sf::IntRect(96, 160, 32, 32),
		sf::IntRect(128, 160, 32, 32),
		sf::IntRect(160, 160, 32, 32)
	};

	sf::IntRect m_ColonCoords = sf::IntRect(0, 192, 32, 32);
	sf::IntRect m_DashCoords = sf::IntRect(32, 192, 32, 32);
	HighScoreManager* m_HighscoreManager;
};