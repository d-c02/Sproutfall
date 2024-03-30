#pragma once
#include "SFML_Lib.h"
#include "Shell.h"
class ShellManager : public sf::Drawable
{
public:
	ShellManager();
	void Update(float tf);
	void SetColor(sf::Color color);
	void CreateShell(sf::Vector2f position, sf::Vector2f direction);
	void SetGravity(float gravity);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	vector<std::unique_ptr<Shell>> m_ShellVector;
	std::unique_ptr<sf::Texture> m_ShellTexture;
	float m_Gravity = 500;
	sf::Color m_Color = sf::Color::White;
};