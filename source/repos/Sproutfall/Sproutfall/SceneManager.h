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
	void loadSceneWithTransition(int scene);
	void Update(float tf);
	void handleInput(sf::Event* event);
	void handleResize(int width, int height);
	void setWindow(sf::RenderWindow* window);
	bool isFullscreen();
private:
	void loadTitle();
	void loadSpace();
	void loadSky();
	void loadForest();
	void loadWin();
	void borderView(int width, int height);
	void configureUI();
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
	
	//UI Stuff
	enum UI {UI_Title_Main, UI_Title_Options, UI_Gameplay_HUD, UI_Gameplay_Paused};

	bool m_LoadSpace = false;
	bool m_LoadSky = false;
	bool m_LoadForest = false;
	bool m_QuitGame = false;
	bool m_OpenOptions = false;
	bool m_CloseOptions = false;

	float m_SFXVolumeSlider = 50.0f;
	bool m_SFXVolumeSliderHeld = false;

	float m_MusicVolumeSlider = 50.0f;
	bool m_MusicVolumeSliderHeld = false;

	bool m_ClosePauseMenu = false;
	bool m_LoadTitle = false;
	bool m_Retry = false;

	bool m_Fullscreen = false;

	bool m_Paused = false;

	std::unique_ptr<sf::Music> m_Music;
	std::unique_ptr<sf::SoundBuffer> m_MenuStartSoundBuffer;
	std::unique_ptr<sf::Sound> m_MenuStartSound;
	std::unique_ptr<sf::SoundBuffer> m_TransitionSoundBuffer;
	std::unique_ptr<sf::Sound> m_TransitionSound;
	bool m_StartSoundPlayed = true;

	std::unique_ptr<sf::Texture> m_TransitionTexture;
	std::unique_ptr<sf::Sprite> m_TransitionSprite;
	bool m_Transitioning = false;
	bool m_TransitionFlipped = false;
	float m_TransitionSpeed = -2000.0f;
	int m_nextScene = 0;
};