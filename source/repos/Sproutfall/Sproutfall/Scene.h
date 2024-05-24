#include "SFML_Lib.h"
#include "BackgroundLayer.h"
#include "BackgroundElement.h"

#pragma once
class Scene : public sf::Drawable
{
public:
	Scene(Player* player, float viewSizeX, float viewSizeY, int levelSize = 0);
	~Scene();
	void addBackground(float StartPos, float EndPos, string texturePath, float levelSize, bool drawOthers = true);
	void addBackgroundElement(sf::Vector2f position, float endPosition, string texturePath, vector<sf::IntRect> frameVector, float frameDelay);
	void setBackgroundFillColor(unsigned int color);
	void UpdateBackgroundPositions(float playerPos);
	int getLevelSize();
	void setParallax(bool parallax);
	bool getParallax();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<std::unique_ptr<BackgroundLayer>> m_Backgrounds;
	unsigned int m_BackgroundFillColor = 0x000000ff;
	Player* m_Player;
	float m_viewSizeX;
	float m_viewSizeY;
	int m_LevelSize;
	vector<std::unique_ptr<sf::Sprite>> m_BackgroundSprites;
	vector<std::unique_ptr<sf::Texture>> m_BackgroundTextures;
	bool m_Parallax = true;
};