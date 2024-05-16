#pragma once
#include "SFML_Lib.h"
#include "Scene.h"
#include "EnemyManager.h"
class SceneManager : public sf::Drawable
{
public:
	SceneManager(float windowSizeX, float windowSizeY, sf::RenderWindow* window);
	~SceneManager();
	void loadScene(int scene);
	void Update(float tf);
	void handleInput(sf::Event* event);
	void handleResize(int width, int height);
	void setWindow(sf::RenderWindow* window);
private:
	void loadTitle();
	void loadSpace();
	void loadSky();
	void loadGround();
	void loadWin();
	void borderView(int width, int height);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::unique_ptr<sf::RectangleShape> m_LeftBorder;
	std::unique_ptr<sf::RectangleShape> m_RightBorder;
	std::unique_ptr<sf::RectangleShape> m_UpperBorder;
	std::unique_ptr<sf::RectangleShape> m_LowerBorder;
	std::unique_ptr<Scene> m_Scene;
	std::unique_ptr<sf::View> m_View;
	std::unique_ptr<EnemyManager> m_EnemyManager;
	enum Behaviors { b_Asteroid, b_Squid, b_Cloud, b_Bird, b_Branch, b_Bug };
	enum Scenes {TitleScreen, Space, Sky, Ground, Win};
	int m_CurrentScene = 0;
	const float m_viewSizeX = 1280;
	const float m_viewSizeY = 960;
	std::unique_ptr<Player> m_Player;
	bool m_loadNextScene = false;
	std::unique_ptr<sf::Sprite> m_playerSmoke;
	std::unique_ptr<sf::Texture> m_smokeTexture;
	std::unique_ptr<AnimationManager> m_smokeAnimationManager;
	sf::RenderWindow* m_renderWindow;

	float m_ScreenShakeSizeX = 0.0f;
	float m_ScreenShakeSizeY = 0.0f;
	float m_TotalScreenShakeTime = 0.05f;
	float m_ScreenShakeTick = 0.001f;
	float m_CurrentScreenShakeTime = 0.0f;
	bool m_ScreenShaking = false;
	float m_CurrentScreenShakeTick = 0.0f;
	sf::Vector2f m_ScreenShakeOffset = sf::Vector2f(0, 0);
};