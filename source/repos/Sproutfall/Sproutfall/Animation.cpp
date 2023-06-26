#include "Animation.h"

Animation::Animation(vector<sf::IntRect> frames, bool autoplay, float frameDelay)
{
	for (int i = 0; i < frames.size(); i++)
	{
		m_Frames.push_back(make_unique<sf::IntRect>(frames[i].left, frames[i].top, frames[i].width, frames[i].height));
	}
	m_FrameDelay = frameDelay;
	m_FrameTime = 0;
	m_Autoplay = autoplay;
	m_CurrentFrame = 0;
	if (m_Autoplay)
	{
		m_Playing = true;
	}
	else
		m_Playing = false;
}
Animation::~Animation()
{
	/*for (int i = 0; i < m_Frames.size(); i++)
	{
		delete(m_Frames[i]);
	}*/
}

bool Animation::getAutoplay()
{
	return m_Autoplay;
}


void Animation::Update(float tf)
{
	if (m_FrameTime >= m_FrameDelay)
	{
		if (m_CurrentFrame < m_Frames.size() - 1)
		{
			m_CurrentFrame += 1;
		}
		else if (m_Autoplay)
		{
			m_CurrentFrame = 0;
		}
		else
		{
			m_Playing = false;
		}
		m_FrameTime = 0;
	}
	m_FrameTime += tf;
}

void Animation::Play()
{
	m_CurrentFrame = 0;
	m_Playing = true;
}

sf::IntRect Animation::getFrame()
{
	return *m_Frames[m_CurrentFrame];
}

bool Animation::isPlaying()
{
	return m_Playing;
}