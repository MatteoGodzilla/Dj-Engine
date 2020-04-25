#include "AnimationManager.h"

void AnimationManager::init() {
	Animation anim1(AN_CROSS_GREEN_TO_LEFT, 0.100);
	Animation anim2(AN_CROSS_GREEN_TO_CENTER, 0.100);
	Animation anim3(AN_CROSS_BLUE_TO_RIGHT, 0.100);
	Animation anim4(AN_CROSS_BLUE_TO_CENTER, 0.100);

	Animation anim5(AN_GREEN_CLICKER, 0.200);
	Animation anim6(AN_RED_CLICKER, 0.200);
	Animation anim7(AN_BLUE_CLICKER, 0.200);

	m_animList.push_back(anim1);
	m_animList.push_back(anim2);
	m_animList.push_back(anim3);
	m_animList.push_back(anim4);
	m_animList.push_back(anim5);
	m_animList.push_back(anim6);
	m_animList.push_back(anim7);
}

void AnimationManager::tick(double time) {
	for (auto& anim : m_animList) {
		anim.tick(time);
	}
}

std::vector<Animation> AnimationManager::getAnimList() {
	return m_animList;
}

Animation AnimationManager::getAnimById(int id) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			return anim;
		}
	}
	std::cerr << "Animation Manager Error : Animation not found" << std::endl;
	return Animation(-1, 0.0);
}

void AnimationManager::updateAnimation(int id, Animation a) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			anim = a;
			break;
		}
	}
}

void AnimationManager::triggerAnimation(int id, double time) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			anim.enable(time);
			anim.reset();
		}
	}
}

void AnimationManager::disableAnimation(int id) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			anim.disable();
			anim.reset();
		}
	}
}
