#pragma once
#include "SFML_Lib.h"
#include "Player.h"
class BackgroundLayer : public sf::Drawable
{
public:
	BackgroundLayer(Player* player, float parallaxSpeed, float viewSizeX, float viewSizeY, string texturePath);
	~BackgroundLayer();
	void Update(float tf);
	void setScale(float x, float y);
	void setParallax(bool parallax);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_ParallaxSpeed = 0;
	int m_Parallax = 1;
	std::unique_ptr<sf::Texture> m_Texture;
	std::unique_ptr<sf::Sprite> m_Sprite;
	std::unique_ptr<sf::Sprite> m_Upper;
	std::unique_ptr<sf::Sprite> m_Lower;
	float m_ViewSizeX;
	float m_ViewSizeY;
	Player* m_Player;
};