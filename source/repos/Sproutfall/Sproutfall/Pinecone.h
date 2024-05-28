#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"

class Pinecone : public sf::Drawable, public Enemy
{
public:
	Pinecone(sf::Texture* texture, Player* player);
	void Update(float tf);
	bool checkProjectiles() override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	enum States {neutral, falling, exploding};
	Player* m_Player;
	int m_State;
};