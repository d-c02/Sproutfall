#pragma once
#include "SFML_Lib.h"

class Timer : public sf::Drawable
{
public:
	Timer(sf::Texture* Texture, sf::Vector2f position);
	void setPosition(sf::Vector2f pos);
	void Update(float tf);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int m_Seconds = 0;
	unsigned int m_Minutes = 0;
	int m_numMinLetters = 1;
	float m_SecondCounter = 0;
	sf::Vector2f m_Position;
	vector<unique_ptr<sf::Sprite>> m_Sprites;
	const float m_horizontalLetterOffset = 64;
	const sf::IntRect m_DisplayPos[10] = {
		sf::IntRect(128, 256, 64, 64),
		sf::IntRect(192, 256, 64, 64), 
		sf::IntRect(256, 256, 64, 64),
		sf::IntRect(320, 256, 64, 64), 
		sf::IntRect(0, 320, 64, 64),
		sf::IntRect(64, 320, 64, 64),
		sf::IntRect(128, 320, 64, 64),
		sf::IntRect(192, 320, 64, 64),
		sf::IntRect(256, 320, 64, 64),
		sf::IntRect(320, 320, 64, 64)};
	const sf::IntRect m_ColonTextureRect = sf::IntRect(0, 384, 64, 64);
	sf::Texture* m_Texture;
};