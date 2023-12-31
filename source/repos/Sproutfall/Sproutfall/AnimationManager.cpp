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
}
void AnimationManager::addState(int state, vector<sf::IntRect> frames, bool autoplay, float frameDelay)
{
	m_States.push_back(make_unique<Animation>(frames, autoplay, frameDelay));
}
void AnimationManager::setState(int state)
{
	m_CurrentState = state;
	if (!m_States[m_CurrentState]->getAutoplay())
	{
		m_States[m_CurrentState]->Play();
	}
}

void AnimationManager::Play()
{
	m_States[m_CurrentState]->Play();
}

void AnimationManager::Update(float tf)
{
	m_States[m_CurrentState]->Update(tf);
	m_Sprite->setTextureRect(m_States[m_CurrentState]->getFrame());
}

bool AnimationManager::isPlaying()
{
	return m_States[m_CurrentState]->isPlaying();
}