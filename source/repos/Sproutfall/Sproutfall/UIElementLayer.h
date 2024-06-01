#pragma once
#include "SFML_lib.h"
#include "UIElement.h"
#include "AnimationManager.h"
#include "UIButton.h"

class UIElementLayer : public sf::Drawable
{
public:
	UIElementLayer();
	void setCurrent(bool val);
	bool isCurrent();
	void Update(float tf);
	void AddVisualElement(string texturePath, sf::Vector2f position);
	void AddAnimatedVisualElement(string texturePath, sf::Vector2f position, vector<sf::IntRect> frameVector);
	void SetPosition(sf::Vector2f pos);
	void AddButton(string TexturePath, bool* switchBool, sf::Vector2f position, sf::IntRect unclickedTextureCoords, sf::IntRect clickedTextureCoords, sf::IntRect releasedTextureCoords, sf::Vector2f hitboxOffset = sf::Vector2f(0, 0));
	void checkClick(sf::Vector2f mousePos);
	void checkClickRelease(sf::Vector2f mousePos);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool m_IsCurrent = false;
	vector<std::unique_ptr<UIElement>> m_UIElements;
};