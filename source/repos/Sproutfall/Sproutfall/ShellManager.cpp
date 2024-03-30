#include "ShellManager.h"

ShellManager::ShellManager()
{
	m_ShellTexture = make_unique<sf::Texture>();
	if (!m_ShellTexture->loadFromFile("Textures/Shell.png"))
	{
		cout << "Shell texture load failure\n";
	}
}

void ShellManager::Update(float tf)
{
	for (int i = 0; i < m_ShellVector.size(); i++)
	{
		m_ShellVector[i]->Update(tf);
	}
	if (m_ShellVector.size() > 0)
	{
		if (m_ShellVector[m_ShellVector.size() - 1]->CanDespawn())
		{
			m_ShellVector.erase(m_ShellVector.begin() + m_ShellVector.size() - 1);
		}
	}
}

void ShellManager::SetColor(sf::Color color)
{
	m_Color = color;
}

void ShellManager::CreateShell(sf::Vector2f position, sf::Vector2f direction)
{
	m_ShellVector.push_back(make_unique<Shell>(m_ShellTexture.get(), m_Color, m_Gravity, position, direction));
}

void ShellManager::SetGravity(float gravity)
{
	m_Gravity = gravity;
}

void ShellManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_ShellVector.size(); i++)
	{
		target.draw(*m_ShellVector[i]);
	}
}