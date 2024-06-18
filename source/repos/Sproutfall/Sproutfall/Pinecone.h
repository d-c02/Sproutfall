#pragma once
#include "SFML_Lib.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Player.h"
#include "EnemyBulletManager.h"

class Pinecone : public sf::Drawable, public Enemy
{
public:
	Pinecone(sf::Texture* texture, sf::Texture* bulletTexture, Player* player, sf::Shader* whiteShader);
	void Update(float tf);
	bool checkProjectiles() override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	enum States {neutral, falling, exploding};
	Player* m_Player;
	int m_State;
	sf::Texture* m_bulletTexture;
	unique_ptr<EnemyBulletManager> m_bulletManager;
	unique_ptr<AnimationManager> m_animationManager;

	sf::Shader* m_whiteShader;

	//DELETE
	float m_currentFireTime = 0.0;
	float m_fireTimeCooldown = 2.0;
};