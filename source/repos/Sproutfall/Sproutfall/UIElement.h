#pragma once
#include "SFML_Lib.h"

class UIElement : public sf::Drawable
{
public:
	UIElement();
	UIElement(string TexturePath);
	virtual void Update(float tf);
	virtual void setPosition(sf::Vector2f pos);
	virtual void checkClick(sf::Vector2f mousePos);
	virtual void checkClickRelease(sf::Vector2f mousePos);

protected:
	std::unique_ptr<sf::Sprite> m_Sprite;
	std::unique_ptr<sf::Texture> m_Texture;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};