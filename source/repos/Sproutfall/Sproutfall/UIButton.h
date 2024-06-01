#pragma once
#include "SFML_Lib.h"
#include "UIElement.h"

class UIButton : public sf::Drawable, public UIElement
{
public:
	UIButton(string TexturePath, bool* switchBool, sf::Vector2f position, sf::IntRect unclickedTextureCoords, sf::IntRect clickedTextureCoords, sf::IntRect releasedTextureCoords, sf::Vector2f hitboxOffset = sf::Vector2f(0, 0));
	void setPosition(sf::Vector2f pos) override;
	void Update(float tf) override;
	void checkClick(sf::Vector2f pos) override;
	void checkClickRelease(sf::Vector2f pos) override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::FloatRect m_Hitbox;
	sf::Vector2f m_HitboxOffset;
	sf::IntRect m_ClickedTextureCoords;
	sf::IntRect m_UnclickedTextureCoords;
	sf::IntRect m_ReleasedTextureCoords;
	bool* m_Switch;
};