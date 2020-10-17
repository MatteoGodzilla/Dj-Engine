#include "TimerManager.h"

Timer::Timer() {
	m_id = -1;
	m_duration = 0.0;
	m_looping = false;
}

Timer::Timer(int id, double duration, bool looping) {
	m_id = id;
	m_duration = duration;
	m_looping = looping;
}

void Timer::tick(double time) {
	//time is global time, not delta time
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

void Timer::enable(double time) {
	//time is global, not delta
	m_enabled = true;
	m_pastTick = time;
}

void Timer::disable() {
	m_enabled = false;
}

void Timer::reset() {
	m_time = 0.0;
}

void Timer::setDuration(double time) {
	m_duration = time;
}

void Timer::setLooping(bool loop) {
	m_looping = loop;
}

int Timer::getId() const {
	return m_id;
}

bool Timer::isLooping() const {
	return m_looping;
}

bool Timer::isEnabled() const {
	return m_enabled;
}

double Timer::getTime() const {
	return m_time;
}

double Timer::getPercent() const {
	return m_time / m_duration;
}

double Timer::getDuration() const {
	return m_duration;
}

void TimerManager::tick(double time) {
	for (auto& anim : m_animList) {
		anim.tick(time);
	}
}

std::vector<Timer>& TimerManager::getAnimList() {
	return m_animList;
}

Timer TimerManager::getAnimById(int id) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			return anim;
		}
	}
	std::cerr << "Timer Manager Error : Animation not found" << std::endl;
	return Timer(-1, 0.0);
}

void TimerManager::updateTimer(Timer a) {
	for (auto& anim : m_animList) {
		if (anim.getId() == a.getId()) {
			anim = a;
			break;
		}
	}
}

void TimerManager::pushTimer(Timer a) {
	m_animList.push_back(a);
}

void TimerManager::triggerTimer(int id, double time) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			anim.enable(time);
			anim.reset();
		}
	}
}

void TimerManager::disableTimer(int id) {
	for (auto& anim : m_animList) {
		if (anim.getId() == id) {
			anim.disable();
			anim.reset();
		}
	}
}
