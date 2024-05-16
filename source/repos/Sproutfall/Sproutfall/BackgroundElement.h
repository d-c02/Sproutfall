#pragma once
#include "SFML_Lib.h"
#include "Player.h"
#include "BackgroundLayer.h"
#include "AnimationManager.h"

class BackgroundElement : public sf::Drawable, public BackgroundLayer
{
public:
	BackgroundElement(Player* player, sf::Vector2f position, float parallaxSpeed, string texturePath, vector<sf::IntRect> frameVector, float frameDelay, bool holdFlush, float vertHoldBoundary);
	void Update(float tf) override;
	void setScale(float x, float y);
	void setPosition(float x, float y);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	bool m_holdFlush = false;
	float m_holdVert = 0.0f;
};