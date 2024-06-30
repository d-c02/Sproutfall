#pragma once
#include "SFML_Lib.h"
#include "Scene.h"

class FinalCutscene : public Scene
{
public:
	FinalCutscene(float viewSizeX, float viewSizeY);
	void Update(float tf) override;
	bool isScreenShaking() override;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	int m_State;
	enum States {s_PlayerFalling, s_ImpactVFX, s_Shaking, s_SproutGrowing};
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
};