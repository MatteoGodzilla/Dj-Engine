#include "Animation.h"
#include <iostream>
#include <vector>

enum AnimId {
	AN_CROSS_GREEN_TO_LEFT,AN_CROSS_GREEN_TO_CENTER,
	AN_CROSS_BLUE_TO_RIGHT,AN_CROSS_BLUE_TO_CENTER
};

class AnimationManager {
public:
	AnimationManager();
	void init(double time);
	void tick(double time);
	std::vector<Animation> getAnimList();
	Animation getAnimById(int id);
	void updateAnimation(int id, Animation a);
	~AnimationManager();
private:
	std::vector<Animation> m_animLisit;
};