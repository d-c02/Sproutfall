#include "Scene.h"

Scene::Scene(Player* player, float viewSizeX, float viewSizeY, int numScreens)
{
	m_Player = player;
	m_viewSizeX = viewSizeX;
	m_viewSizeY = viewSizeY;
	m_NumScreens = numScreens;
}
Scene::~Scene()
{
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		delete(m_Backgrounds[i]);
	}
	for (int i = 0; i < m_BackgroundSprites.size(); i++)
	{
		delete(m_BackgroundSprites[i]);
		delete(m_BackgroundTextures[i]);
	}
}

void Scene::addBackground(float parallaxSpeed, sf::Texture* texture, int screenNumber)
{
	if (screenNumber < 0)
	{
		m_Backgrounds.push_back(new BackgroundLayer(m_Player, parallaxSpeed, m_viewSizeX, m_viewSizeY, texture));
		m_Backgrounds[m_Backgrounds.size() - 1]->setScale(2, 2);
	}
	else
	{
		sf::Sprite* sprite = new sf::Sprite();
		m_BackgroundSprites.push_back(sprite);
		m_BackgroundTextures.push_back(texture);
		sprite->setTexture(*texture);
		sprite->setScale(2, 2);
		sprite->setPosition(0, m_viewSizeY * screenNumber);
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
	return m_NumScreens;
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
		m_Backgrounds[i]->setParallax(false);
	}
}
bool Scene::getParallax()
{
	return m_Parallax;
}