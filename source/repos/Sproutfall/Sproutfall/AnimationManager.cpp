#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include "AnimationManager.h"
AnimationManager::AnimationManager(sf::Sprite* sprite)
{
	m_Sprite = sprite;
	m_CurrentState = 0;
}
AnimationManager::~AnimationManager()
{
	/*for (int i = 0; i < m_States.size(); i++)
	{
		delete m_States[i];
	}*/
	_CrtDumpMemoryLeaks();
}
void AnimationManager::addState(int state, vector<sf::IntRect> frames, bool autoplay, float frameDelay)
{
	m_States.push_back(make_unique<Animation>(frames, autoplay, frameDelay));
}
void AnimationManager::setState(int state)
{
	m_CurrentState = state;
}
void AnimationManager::Update(float tf)
{
	m_States[m_CurrentState]->Update(tf);
	m_Sprite->setTextureRect(m_States[m_CurrentState]->getFrame());
}