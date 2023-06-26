#pragma once
#include "SFML_Lib.h"
class Animation
{
public:
	Animation(vector<sf::IntRect> frames, bool autoplay, float frameDelay);
	~Animation();
	void Update(float tf);
	sf::IntRect getFrame();
	void Play();
	bool isPlaying();
	bool getAutoplay();
private:
	vector<std::unique_ptr<sf::IntRect>> m_Frames;
	bool m_Autoplay;
	int m_CurrentFrame;
	float m_FrameDelay;
	float m_FrameTime = 0;
	bool m_Playing = false;
};