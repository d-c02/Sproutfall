#pragma once
#include "SFML_Lib.h"
#include "UIElement.h"

class UISlider : public sf::Drawable, public UIElement
{
public:
	UISlider(sf::Vector2f position, float* sliderVal, string barTexturePath, string sliderNubTexturePath, float minVal, float maxVal, sf::Vector2f m_HitboxOffset = sf::Vector2f(0,0));
	void setPosition(sf::Vector2f pos) override;
	void Update(float tf) override;
	void checkClick(sf::Vector2f pos) override;
	void checkClickRelease(sf::Vector2f pos) override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::FloatRect m_Hitbox;
	sf::Vector2f m_HitboxOffset;

	std::unique_ptr<sf::Sprite> m_NubSprite;
	std::unique_ptr<sf::Texture> m_NubTexture;
	float* m_sliderVal;
	float m_minVal;
	float m_maxVal;
};