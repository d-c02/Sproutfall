#pragma once
#include "SFML_Lib.h"
#include "Player.h"
class BackgroundLayer : public sf::Drawable
{
public:
	BackgroundLayer();
	BackgroundLayer(Player* player, float parallaxSpeed, float viewSizeX, float viewSizeY, string texturePath, bool drawOthers, float verticalOffset);
	~BackgroundLayer();
	virtual void Update(float tf);
	void setScale(float x, float y);
	void setParallax(bool parallax);
protected:
	Player* m_Player;
	float m_ParallaxSpeed = 0;
	int m_Parallax = 1;
	std::unique_ptr<sf::Texture> m_Texture;
	std::unique_ptr<sf::Sprite> m_Sprite;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unique_ptr<sf::Sprite> m_Upper;
	std::unique_ptr<sf::Sprite> m_Lower;
	float m_ViewSizeX;
	float m_ViewSizeY;
	float m_verticalOffset = 0;
	bool m_drawOthers;
};