#pragma once
#include "SFML_Lib.h"
#include "Scene.h"
class SceneManager : public sf::Drawable
{
public:
	SceneManager(sf::IntRect view);
	~SceneManager();
	void addScene(int scene);
	void setScene(int scene);
	void Update(float tf);
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	vector<Scene*> m_Scenes;
	int m_CurrentScene;
	const int m_Level_Size = 10;
	sf::IntRect* m_View;
};