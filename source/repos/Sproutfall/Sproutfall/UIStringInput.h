#pragma once
#include "SFML_Lib.h"
#include "UIElement.h"

class UIStringInput : public UIElement
{
public:
	UIStringInput(sf::Texture* atlasTexture, string* modifiedString, int maxLength, sf::Vector2f position);
	void setPosition(sf::Vector2f pos) override;
	void handleInput(sf::Event* event) override;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<unique_ptr<sf::Sprite>> m_Sprites;

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

	sf::IntRect m_DashCoords = sf::IntRect(32, 192, 32, 32);

	int m_Length;
	sf::Vector2f m_Position;
	sf::Texture* m_atlasTexture;

	int m_CurMod = 0;
	int m_DelMod = 0;
};