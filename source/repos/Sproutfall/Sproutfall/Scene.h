#include "SFML_Lib.h"
#include "BackgroundLayer.h"
#pragma once
class Scene : public sf::Drawable
{
public:
	Scene(Player* player, float viewSizeX, float viewSizeY, int numScreens = 0);
	~Scene();
	void addBackground(float parallaxSpeed, sf::Texture* texture, int m_screenNumber = -1);
	void setBackgroundFillColor(unsigned int color);
	void Update(float tf);
	int getLevelSize();
	void setParallax(bool parallax);
	bool getParallax();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<BackgroundLayer*> m_Backgrounds;
	unsigned int m_BackgroundFillColor = 0x000000ff;
	Player* m_Player;
	float m_viewSizeX;
	float m_viewSizeY;
	int m_NumScreens;
	vector<sf::Sprite*> m_BackgroundSprites;
	vector<sf::Texture*> m_BackgroundTextures;
	bool m_Parallax = true;
};