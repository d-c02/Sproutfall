#pragma once
#include "SFML_Lib.h"
#include "Scene.h"
class SceneManager : public sf::Drawable
{
public:
	SceneManager(Player* player, float viewSizeX, float viewSizeY);
	~SceneManager();
	void loadScene(int scene);
	void Update(float tf);
private:
	void loadTitle();
	void loadSpace();
	void loadSky();
	void loadGround();
	void loadWin();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Scene* m_Scene;
	sf::View* m_View;
	enum Scenes {TitleScreen, Space, Sky, Ground, Win};
	float m_viewSizeX;
	float m_viewSizeY;
	Player* m_Player;
};