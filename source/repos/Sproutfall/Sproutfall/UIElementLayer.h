#pragma once
#include "SFML_lib.h"
#include "UIElement.h"
#include "AnimationManager.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UICounter.h"
#include "UIStringInput.h"

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
	void AddButton(string TexturePath, bool* switchBool, sf::Vector2f position, sf::IntRect unclickedTextureCoords, sf::IntRect clickedTextureCoords, sf::IntRect releasedTextureCoords, sf::RenderWindow* window, sf::Vector2f hitboxOffset = sf::Vector2f(0, 0));
	void AddSlider(sf::Vector2f position, float* sliderVal, bool* buttonHeld, string barTexturePath, string sliderNubTexturePath, float minVal, float maxVal, sf::RenderWindow* window, sf::Vector2f m_HitboxOffset = sf::Vector2f(0, 0));
	void AddCounter(string TexturePath, int* countVal, int maxVal, sf::IntRect ActiveTextureCoords, sf::IntRect InactiveTextureCoords, sf::Vector2f position);
	void AddStringInput(sf::Texture* atlasTexture, string* modifiedString, int maxLength, sf::Vector2f position);

	void handleInput(sf::Event* event);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool m_IsCurrent = false;
	vector<std::unique_ptr<UIElement>> m_UIElements;
};