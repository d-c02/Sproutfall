#pragma once
#include "SFML_Lib.h"
#include "Player.h"
class BackgroundLayer : public sf::Drawable
{
public:
	BackgroundLayer();
	BackgroundLayer(float StartPos, float EndPos, float viewSizeX, float viewSizeY, string texturePath, float levelSize, bool drawOthers);
	~BackgroundLayer();
	virtual void UpdatePosition(float ypos);
	void setScale(float x, float y);
	void setParallax(bool parallax);
protected:
	float m_StartPosition = 0;
	float m_EndPosition = 0;
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
	float m_levelSize = 0;
	float m_TruePosition = 0;
	float m_ParallaxShift = 0;
};