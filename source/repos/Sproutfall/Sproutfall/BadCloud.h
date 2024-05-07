#pragma once

#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"

class BadCloud : public sf::Drawable, public Enemy
{
public:
	BadCloud(sf::Texture* texture, Player* player);
	void Update(float tf);
	bool checkProjectiles() override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; \
	void configureAnimations();
	std::unique_ptr<AnimationManager> m_AnimationManager;
	enum States { neutral, attacking};
	sf::RectangleShape m_belowCollider;
	sf::RectangleShape m_lightningCollider;
	std::unique_ptr<sf::Sprite> m_lightningSprite;
	const float m_attackTime = 0.5f;
	float m_currentAttackTime = 0.0f;
	Player* m_Player;
	int m_State;
};