#pragma once
#include "SFML_Lib.h"
#include "Animation.h"

class AnimationManager
{
public:
	AnimationManager(sf::Sprite* sprite);
	~AnimationManager();
	void addState(int state, vector<sf::IntRect> frames, bool autoplay, float frameDelay);
	void setState(int state);
	void Update(float tf);
private:
	sf::Sprite* m_Sprite;
	vector<Animation*> m_States;
	int m_CurrentState = 0;
};