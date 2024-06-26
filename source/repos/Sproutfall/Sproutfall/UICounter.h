#pragma once
#include "SFML_Lib.h"
#include "UIElement.h"

class UICounter : public UIElement
{
public:
	UICounter(string TexturePath, int* countVal, int maxVal, sf::IntRect ActiveTextureCoords, sf::IntRect InactiveTextureCoords, sf::Vector2f position);
	void setPosition(sf::Vector2f pos) override;
	void Update(float tf) override;
private:
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPrevCountVal();
	sf::IntRect m_ActiveTextureCoords;
	sf::IntRect m_InactiveTextureCoords;
	int* m_countVal;
	int m_prevCountVal;
	int m_maxVal;

	vector<unique_ptr<sf::Sprite>> m_CounterSprites;
};