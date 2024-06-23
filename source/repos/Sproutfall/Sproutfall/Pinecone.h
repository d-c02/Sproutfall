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
	void configureAnimations();
	enum States {neutral, exploding, falling, exploded};
	int m_CurrentState;
	Player* m_Player;
	int m_State;
	sf::Texture* m_bulletTexture;
	unique_ptr<EnemyBulletManager> m_bulletManager;
	unique_ptr<AnimationManager> m_animationManager;
	vector<unique_ptr<sf::Sprite>> m_ExplosionVFX;
	vector<unique_ptr<AnimationManager>> m_ExplosionAnimationManagers;
	sf::Texture* m_ExplosionTexture;
	sf::Shader* m_whiteShader;

	const float m_ExplosionVFXRange = 32.0f;

	const float m_terminalVelocity = 825;
	const float m_gravity = 500;
	const float m_airResistance = -250;
	
	float m_currentFallingTime = 0.0;
	const float m_maxFallingTime = 2.0f;

	float m_deathTime = 0.0;
	const float m_maxDeathTime = 1.5f;
};