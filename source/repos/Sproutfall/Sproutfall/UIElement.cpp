#include "UIElement.h"

UIElement::UIElement()
{
}

UIElement::UIElement(string texturePath, sf::Vector2f position)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Texture = make_unique<sf::Texture>();

	if (!m_Texture->loadFromFile(texturePath))
	{
		cout << "Visual UI element texture load failure";
	}

	m_Sprite->setTexture(*m_Texture);
	m_Sprite->setScale(2, 2);

	m_UIPosition = position;
	m_Sprite->setPosition(m_UIPosition);
}

void UIElement::Update(float tf)
{

}

void UIElement::setPosition(sf::Vector2f pos)
{
	m_Sprite->setPosition(m_UIPosition + pos);
}

void UIElement::handleInput(sf::Event* event)
{
	//Do nothing
}

void UIElement::updateHighScores()
{
	//Do nothing
}

void UIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}
