#pragma once
#include "SFML_Lib.h"
#include "Player.h"
#include "BackgroundLayer.h"
#include "AnimationManager.h"

class BackgroundElement : public sf::Drawable, public BackgroundLayer
{
public:
	BackgroundElement(sf::Vector2f position, float endPosition, string texturePath, vector<sf::IntRect> frameVector, float frameDelay, float levelSize, float verticalOffset);
	void UpdatePosition(float pos) override;
	void setScale(float x, float y);
	void setPosition(float x, float y);
	void Update(float tf) override;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	sf::Vector2f m_StartPosition;
	float m_EndPositionY = 0;
	float m_levelSize = 0;
	float m_verticalOffset;
};