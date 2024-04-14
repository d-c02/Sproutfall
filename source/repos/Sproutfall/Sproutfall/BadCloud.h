#pragma once

#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"

class BadCloud : public sf::Drawable, public Enemy
{
public:
	BadCloud(sf::Texture* texture);
	void Update(float tf);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};