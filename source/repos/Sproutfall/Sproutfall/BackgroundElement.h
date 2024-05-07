#pragma once
#include "SFML_Lib.h"
#include "Player.h"
#include "BackgroundLayer.h"
#include "AnimationManager.h"

class BackgroundElement : public sf::Drawable, public BackgroundLayer
{
public:
	BackgroundElement(Player* player, float parallaxSpeed, string texturePath, AnimationManager animationManager);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float m_ParallaxSpeed = 0;
	std::unique_ptr<sf::Texture> m_Texture;
	std::unique_ptr<sf::Sprite> m_Sprite;

};