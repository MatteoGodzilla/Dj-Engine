#include "Animation.h"

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

int Animation::getId() {
	return m_id;
}

bool Animation::isLooping() {
	return m_looping;
}

bool Animation::isEnabled() {
	return m_enabled;
}

double Animation::getTime() {
	return m_time;
}

double Animation::getPercent() {
	return m_time / m_duration;
}

double Animation::getDuration() {
	return m_duration;
}

Animation::~Animation() {
}