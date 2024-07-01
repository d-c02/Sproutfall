#pragma once
#include "SFML_Lib.h"
#include "Scene.h"

class FinalCutscene : public Scene
{
public:
	FinalCutscene(float viewSizeX, float viewSizeY, sf::Texture* TransitionTexture);
	void Update(float tf) override;
	bool isScreenShaking() override;
	sf::Vector2f getViewCenter() override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void loadScene(int Scene);
	void loadForest();
	void loadSky();
	void loadSpace();
	void loadFarout();
	void UpdateTransitionPos();

	int m_State;
	enum States {s_PlayerFalling, s_ImpactVFX, s_Shaking, s_SproutGrowing, s_Transitioning, s_Forest, s_Sky, s_Space, s_FarOut, s_End};
	sf::Vector2f m_initialPlayerPosition = sf::Vector2f(640, 0);
	sf::Vector2f m_finalPlayerPosition = sf::Vector2f(640, 850);
	float m_TimeAccumulator = 0;
	float m_MaxFallingTime = 5.0f;

	std::unique_ptr<sf::Sprite> m_playerSprite;
	std::unique_ptr<sf::Texture> m_playerTexture;
	std::unique_ptr<AnimationManager> m_playerAnimationManager;

	std::unique_ptr<sf::Sprite> m_SproutSprite;
	std::unique_ptr<sf::Texture> m_SproutTexture;
	std::unique_ptr<AnimationManager> m_SproutAnimationManager;

	bool m_ScreenShaking = false;
	float m_MaxShakingTime = 1.0f;

	sf::Texture* m_TransitionTexture;
	std::unique_ptr<sf::Sprite> m_TransitionSprite;
	sf::Vector2f m_initialTransitionPosition = sf::Vector2f(0, -960);
	sf::Vector2f m_finalTransitionPosition = sf::Vector2f(0, 960 / 2);
	sf::Vector2f m_finalTransitionPosition2 = sf::Vector2f(0, 2 * 960);
	float m_MaxTransitionTime = 1.0f;
	bool m_TransitionFlipped = false;
	int m_NextScene = 0;

	long m_BGUpdatePos = 480;
	float m_BGUpdateTick = -400;
	std::unique_ptr<sf::Sprite> m_GrowingSproutSprite;
	std::unique_ptr<sf::Texture> m_GrowingSproutTexture;
	std::unique_ptr<AnimationManager> m_GrowingSproutAnimationManager;

	bool m_HoldingTextPos = false;
	bool m_MovingPastText = false;
	float m_ReadingTime = 5.0f;
	float m_MovingPastTime = 5.0f;
	std::unique_ptr<sf::Texture> m_TextTexture;
	std::unique_ptr<sf::Sprite> m_TextSprite;
	std::unique_ptr<sf::Texture> m_EnemyTexture1;
	std::unique_ptr<sf::Sprite> m_EnemySprite1;
	std::unique_ptr<AnimationManager> m_EnemyAnimationManager1;
	std::unique_ptr<sf::Texture> m_EnemyTexture2;
	std::unique_ptr<sf::Sprite> m_EnemySprite2;
	std::unique_ptr<AnimationManager> m_EnemyAnimationManager2;

	

	float m_CreditsOffset = -2000;
};