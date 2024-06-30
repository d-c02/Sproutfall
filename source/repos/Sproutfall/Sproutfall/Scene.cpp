#include "Scene.h"

Scene::Scene()
{
}

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

void Scene::addBackground(float StartPos, float EndPos, string texturePath, float levelSize, bool drawOthers)
{
	m_Backgrounds.push_back(make_unique<BackgroundLayer>(StartPos, EndPos, m_viewSizeX, m_viewSizeY, texturePath, levelSize, drawOthers));
	m_Backgrounds[m_Backgrounds.size() - 1]->setScale(2, 2);

}

void Scene::addBackgroundElement(sf::Vector2f position, float endPosition, string texturePath, vector<sf::IntRect> frameVector, float frameDelay, float levelSize, float verticalOffset)
{
	m_Backgrounds.push_back(make_unique<BackgroundElement>(position, endPosition, texturePath, frameVector, frameDelay, levelSize, verticalOffset));
}

void Scene::setBackgroundFillColor(unsigned int color)
{
	m_BackgroundFillColor = color;
}

void Scene::UpdateBackgroundPositions(float playerPos)
{
	for (int i = 0; i < m_Backgrounds.size(); i++)
	{
		m_Backgrounds[i]->UpdatePosition(playerPos);
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

void Scene::Update(float tf)
{
	//for (int i = 0; i < m_Backgrounds.size(); i++)
	//{
	//	m_Backgrounds[i]->Update(tf);
	//}
}

void Scene::setGameplay(bool gaming)
{
	m_HasGameplay = gaming;
}

bool Scene::hasGameplay()
{
	return m_HasGameplay;
}

bool Scene::isScreenShaking()
{
	return false;
}
