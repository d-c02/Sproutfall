#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
class Reticle : public sf::Drawable, public Entity
{
public:
	Reticle(sf::Sprite* playerSprite, sf::RenderWindow* window);
	~Reticle();
	void Update(float tf);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RenderWindow* m_window;
	sf::IntRect m_animList[2] = { sf::IntRect(0, 0, 8, 5), sf::IntRect(0, 5, 8, 5) };
	sf::Sprite* m_playerSprite;
	int m_animIndex = 0;
	float m_distanceFromPlayer = 50;
	float m_animCounter = 0;
};