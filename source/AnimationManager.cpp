#include "AnimationManager.h"

AnimationManager::AnimationManager() {
}

void AnimationManager::init(double time) {
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
	for (size_t i = 0; i < m_animList.size(); ++i) {
		m_animList.at(i).tick(time);
	}
}

std::vector<Animation> AnimationManager::getAnimList() {
	return m_animList;
}

Animation AnimationManager::getAnimById(int id) {
	for (size_t i = 0; i < m_animList.size(); ++i) {
		if (m_animList.at(i).getId() == id) {
			return m_animList.at(i);
		}
	}
	std::cerr << "Animation Manager Error : Animation not found" << std::endl;
	return Animation(-1, 0.0);
}

void AnimationManager::updateAnimation(int id, Animation a) {
	for (size_t i = 0; i < m_animList.size(); ++i) {
		if (m_animList.at(i).getId() == id) {
			m_animList.at(i) = a;
			break;
		}
	}
}

void AnimationManager::triggerAnimation(int id, double time) {
	for (size_t i = 0; i < m_animList.size(); ++i) {
		if (m_animList.at(i).getId() == id) {
			m_animList.at(i).enable(time);
			m_animList.at(i).reset();
		}
	}
}

void AnimationManager::disableAnimation(int id){
	for (size_t i = 0; i < m_animList.size(); ++i) {
		if (m_animList.at(i).getId() == id) {
			m_animList.at(i).disable();
			m_animList.at(i).reset();
		}
	}
}

AnimationManager::~AnimationManager() {
}
