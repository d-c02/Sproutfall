#include "UIElementLayer.h"

UIElementLayer::UIElementLayer()
{

}

void UIElementLayer::setCurrent(bool val)
{
	m_IsCurrent = val;
}

bool UIElementLayer::isCurrent()
{
	return m_IsCurrent;
}

void UIElementLayer::Update(float tf)
{
}

void UIElementLayer::AddVisualElement(string texturePath, sf::Vector2f position)
{
	m_UIElements.push_back(make_unique<UIElement>(texturePath));
	m_UIElements[m_UIElements.size() - 1]->setPosition(position);
}

void UIElementLayer::AddAnimatedVisualElement(string texturePath, sf::Vector2f position, vector<sf::IntRect> frameVector)
{

}



void UIElementLayer::SetPosition(sf::Vector2f pos)
{
	for (int i = 0; i < m_UIElements.size(); i++)
	{
		m_UIElements[i]->setPosition(pos);
	}
}

void UIElementLayer::AddButton(string TexturePath, bool* switchBool, sf::IntRect unclickedTextureCoords, sf::IntRect clickedTextureCoords, sf::IntRect releasedTextureCoords, sf::Vector2f hitboxOffset)
{
	m_UIElements.push_back(make_unique<UIButton>(TexturePath, switchBool, unclickedTextureCoords, clickedTextureCoords, releasedTextureCoords, hitboxOffset));
}

void UIElementLayer::checkClick(sf::Vector2f mousePos)
{
	if (m_IsCurrent)
	{
		for (int i = 0; i < m_UIElements.size(); i++)
		{
			m_UIElements[i]->checkClick(mousePos);
		}
	}
}

void UIElementLayer::checkClickRelease(sf::Vector2f mousePos)
{
	if (m_IsCurrent)
	{
		for (int i = 0; i < m_UIElements.size(); i++)
		{
			m_UIElements[i]->checkClickRelease(mousePos);
		}
	}
}

void UIElementLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_IsCurrent)
	{
		for (int i = 0; i < m_UIElements.size(); i++)
		{
			target.draw(*m_UIElements[i]);
		}
	}
}
