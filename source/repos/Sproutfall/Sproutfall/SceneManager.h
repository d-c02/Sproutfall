#pragma once
#include "SFML_Lib.h"
#include "Scene.h"
#include "EnemyManager.h"
class SceneManager : public sf::Drawable
{
public:
	SceneManager(float viewSizeX, float viewSizeY, sf::RenderWindow* window);
	~SceneManager();
	void loadScene(int scene);
	void Update(float tf);
	void handleInput(sf::Event* event);
	void handleResize(int width, int height);
private:
	void loadTitle();
	void loadSpace();
	void loadSky();
	void loadGround();
	void loadWin();
	void removeBullet(int index);
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
	float m_viewSizeX;
	float m_viewSizeY;
	std::unique_ptr<Player> m_Player;
	bool m_loadNextScene = false;
	std::unique_ptr<sf::Sprite> m_playerSmoke;
	std::unique_ptr<sf::Texture> m_smokeTexture;
	std::unique_ptr<AnimationManager> m_smokeAnimationManager;
	sf::RenderWindow* m_renderWindow;
	sf::Text m_FPSCtr;
	sf::Font m_FPSCtrFont;
	int m_FPS = 0;
	int m_PrevFPS = 0;
	float m_FPSTime = 0.0f;
};