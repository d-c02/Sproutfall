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
private:
	void loadTitle();
	void loadSpace();
	void loadSky();
	void loadGround();
	void loadWin();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

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
};