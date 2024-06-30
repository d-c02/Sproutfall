#pragma once
#include "SFML_Lib.h"

class Timer : public sf::Drawable
{
public:
	Timer(sf::Texture* Texture, sf::Vector2f position);
	void setPosition(sf::Vector2f pos);
	void Update(float tf);
	void Reset();
private:
	void Timeout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int m_Seconds = 0;
	unsigned int m_Minutes = 0;
	unsigned int m_Hours = 0;
	int m_minToSecIndex = 1;
	int m_hourToMinIndex = -1;
	float m_SecondCounter = 0;
	sf::Vector2f m_Position;
	vector<unique_ptr<sf::Sprite>> m_Sprites;
	const float m_horizontalLetterOffset = 48;
	float m_leftShift = 0;
	const sf::IntRect m_DisplayPos[10] = {
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
	const sf::IntRect m_ColonTextureRect = sf::IntRect(0, 192, 32, 32);
	const sf::IntRect m_SLOWTextureRects[4] = { 
		sf::IntRect(0, 96, 32, 32),
		sf::IntRect(160, 32, 32, 32),
		sf::IntRect(64, 64, 32, 32),
		sf::IntRect(128, 96, 32, 32) 
	};
	sf::Texture* m_Texture;
	bool m_timedOut = false;
};