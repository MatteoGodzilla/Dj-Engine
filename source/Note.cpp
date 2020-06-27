#include "Note.h"

Note::Note(double milli, int ty, double length, bool ev) {
	//set hit time
	m_hit_window = hitWindow;
	m_milli = milli;
	m_type = ty;
	m_isEvent = ev;
	m_length = length;
}

//when the player clicks the corrisponding button
void Note::click() {
	if (m_firstClick) {
		if (m_hittable) {
			m_touched = true;
			m_hittable = false;
		}
		m_firstClick = false;
	}
}

//updating note every frame
void Note::tick(double time) {
	m_hittable = (m_milli - time <= m_hit_window && time - m_milli <= m_hit_window);
	if (time > m_milli + m_hit_window + m_length && !m_isEvent) {
		m_dead = true;
	}
}

//utility function to return variables
double Note::getMilli() const {
	return m_milli;
}

double Note::getLength() const {
	return m_length;
}

int Note::getType() const {
	return m_type;
}

bool Note::getHit() const {
	return m_hittable;
}
bool Note::getTouched() const {
	return m_touched;
}

void Note::setTouched(bool value) {
	m_touched = value;
}

bool Note::getIsEvent() const {
	return m_isEvent;
}
bool Note::getDead() const {
	return m_dead;
}

int Note::getLanMod() const {
	return m_lan_mod;
}
void Note::setLanMod(int i) {
	m_lan_mod = i;
}
