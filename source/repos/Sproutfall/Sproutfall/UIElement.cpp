#include "UIElement.h"

UIElement::UIElement()
{
}

UIElement::UIElement(string texturePath)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();

	if (!m_Texture->loadFromFile(texturePath))
	{
		cout << "Visual UI element texture load failure";
	}

	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);
}

void UIElement::Update(float tf)
{

}

void UIElement::setPosition(sf::Vector2f pos)
{
	m_Sprite->setPosition(pos);
}

void UIElement::checkClick(sf::Vector2f mousePos)
{
	//Do nothing
}

void UIElement::checkClickRelease(sf::Vector2f mousePos)
{
	//Do nothing
}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}
