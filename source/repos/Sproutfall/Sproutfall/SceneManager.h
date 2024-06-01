#pragma once
#include "SFML_Lib.h"
#include "Scene.h"
#include "EnemyManager.h"
#include "UIElementLayer.h"
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
	void loadForest();
	void loadWin();
	void borderView(int width, int height);
	void handleUIInput();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::unique_ptr<sf::RectangleShape> m_LeftBorder;
	std::unique_ptr<sf::RectangleShape> m_RightBorder;
	std::unique_ptr<sf::RectangleShape> m_UpperBorder;
	std::unique_ptr<sf::RectangleShape> m_LowerBorder;
	std::unique_ptr<Scene> m_Scene;
	std::unique_ptr<sf::View> m_View;
	std::unique_ptr<EnemyManager> m_EnemyManager;
	enum Behaviors { b_Asteroid, b_Squid, b_Cloud, b_Bird, b_Branch, b_Bug, b_Pinecone, b_Squirrel};
	enum Scenes {TitleScreen, Space, Sky, Forest, Win};
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

	vector<std::unique_ptr<UIElementLayer>> m_UILayers;
	
	enum TitleUI {UI_Main, UI_Options};
	bool m_LoadSpace = false;
	bool m_QuitGame = false;
	bool m_OpenOptions = false;
	bool m_OpenMainTitle = false;
	float m_SFXVolumeSlider = 50.0f;
	bool m_SFXVolumeSliderHeld = false;
};