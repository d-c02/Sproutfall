#pragma once
#include "SFML_Lib.h"
#include "Player.h"
class BackgroundLayer : public sf::Drawable
{
public:
	BackgroundLayer(Player* player, float parallaxSpeed, float viewSizeX, float viewSizeY, sf::Texture* texture);
	~BackgroundLayer();
	void Update(float tf);
	void setScale(float x, float y);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_ParallaxSpeed = 0;
	sf::Texture* m_Texture;
	sf::Sprite* m_Sprite;
	sf::Sprite* m_Upper;
	sf::Sprite* m_Lower;
	float m_ViewSizeX;
	float m_ViewSizeY;
	Player* m_Player;
};