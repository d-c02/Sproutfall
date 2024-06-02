#pragma once
#include "SFML_Lib.h"

class UIElement : public sf::Drawable
{
public:
	UIElement();
	UIElement(string TexturePath, sf::Vector2f position);
	virtual void Update(float tf);
	virtual void setPosition(sf::Vector2f pos);
	virtual void handleInput(sf::Event* event);

protected:
	std::unique_ptr<sf::Sprite> m_Sprite;
	std::unique_ptr<sf::Texture> m_Texture;
	sf::Vector2f m_UIPosition;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};