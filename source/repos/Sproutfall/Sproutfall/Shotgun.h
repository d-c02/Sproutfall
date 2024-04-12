#pragma once
#include "SFML_Lib.h"
#include "Entity.h"
#include "AnimationManager.h"
class Shotgun : public sf::Drawable, public Entity
{
public:
	Shotgun(sf::Sprite* playerSprite, sf::RenderWindow* window);
	~Shotgun();
	void Update(float tf);
	void Shoot();
private:
	void configureAnimations();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RenderWindow* m_window;
	std::unique_ptr<AnimationManager> m_AnimationManager;
	sf::Sprite* m_playerSprite;
	float m_distanceFromPlayer = 20;
	enum States {neutral, shooting};
};