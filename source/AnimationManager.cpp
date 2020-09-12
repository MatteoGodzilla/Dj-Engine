#include "AnimationManager.h"

Animation::Animation() {
	m_id = -1;
	m_duration = 0.0;
	m_looping = false;
}

Animation::Animation(int id, double duration, bool looping) {
	m_id = id;
	m_duration = duration;
	m_looping = looping;
}

void Animation::tick(double time) {
	if (m_enabled) {
		m_time += time - m_pastTick;
		if (m_time > m_duration) {
			reset();
			if (!m_looping) {
				disable();
			}
		}
	}
	m_pastTick = time;
}

void Animation::enable(double time) {
	m_enabled = true;
	m_pastTick = time;
}

void Animation::disable() {
	m_enabled = false;
}

void Animation::reset() {
	m_time = 0.0;
}

void Animation::setDuration(double time) {
	m_duration = time;
}

void Animation::setLooping(bool loop) {
	m_looping = loop;
}

int Animation::getId() const {
	return m_id;
}

bool Animation::isLooping() const {
	return m_looping;
}

bool Animation::isEnabled() const {
	return m_enabled;
}

double Animation::getTime() const {
	return m_time;
}

double Animation::getPercent() const {
	return m_time / m_duration;
}

double Animation::getDuration() const {
	return m_duration;
}

void AnimationManager::tick(double time) {
	for (auto& anim : m_animList) {
		anim.tick(time);
	}
}

std::vector<Animation>& AnimationManager::getAnimList() {
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

void AnimationManager::updateAnimation(Animation a) {
	for (auto& anim : m_animList) {
		if (anim.getId() == a.getId()) {
			anim = a;
			break;
		}
	}
}

void AnimationManager::pushAnimation(Animation a) {
	m_animList.push_back(a);
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
