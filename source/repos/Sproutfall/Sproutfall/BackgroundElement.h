#pragma once
#include "SFML_Lib.h"
#include "Player.h"
#include "BackgroundLayer.h"
#include "AnimationManager.h"

class BackgroundElement : public sf::Drawable, public BackgroundLayer
{
public:
	BackgroundElement(Player* player, float parallaxSpeed, string texturePath, AnimationManager animationManager);
	void Update(float tf);
	void setScale(float x, float y);
	void setPosition(float x, float y);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};