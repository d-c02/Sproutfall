#include "Scene.h"

Scene::Scene(Player* player, float viewSizeX, float viewSizeY, int levelSize)
{
	m_Player = player;
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_LevelSize = levelSize;
}
Scene::~Scene()
{

}

void Scene::addBackground(float parallaxSpeed, string texturePath, int screenNumber, bool drawOthers)
{
	if (screenNumber < 0)
	{
		m_Backgrounds.push_back(make_unique<BackgroundLayer>(m_Player, parallaxSpeed, m_viewSizeX, m_viewSizeY, texturePath, drawOthers));
		m_Backgrounds[m_Backgrounds.size() - 1]->setScale(2, 2);
	}
	else
	{
		m_BackgroundSprites.push_back(make_unique<sf::Sprite>());
		m_BackgroundTextures.push_back(make_unique<sf::Texture>());
		m_BackgroundSprites[m_BackgroundSprites.size() - 1]->setTexture(*m_BackgroundTextures[m_BackgroundTextures.size() - 1]);
		m_BackgroundSprites[m_BackgroundSprites.size() - 1]->setScale(2, 2);
		m_BackgroundSprites[m_BackgroundSprites.size() - 1]->setPosition(0, m_viewSizeY * screenNumber);
	}
}

void Scene::setBackgroundFillColor(unsigned int color)
{
	m_BackgroundFillColor = color;
}

void Scene::Update(float tf)
{
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		m_Backgrounds[i]->Update(tf);
	}
}

int Scene::getLevelSize()
{
	return m_LevelSize;
}
void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear(sf::Color(m_BackgroundFillColor));
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		target.draw(*m_Backgrounds[i]);
	}
	for (int i = 0; i < m_BackgroundSprites.size(); i++)
	{
		target.draw(*m_BackgroundSprites[i]);
	}
}

void Scene::setParallax(bool parallax)
{
	m_Parallax = parallax;
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		m_Backgrounds[i]->setParallax(parallax);
	}
}
bool Scene::getParallax()
{
	return m_Parallax;
}