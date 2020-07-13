#pragma once
#include <iostream>

enum note {
	TAP_G,
	TAP_R,
	TAP_B,
	CROSS_G,
	CROSS_B,
	CROSS_C,
	CF_SPIKE_G,
	CF_SPIKE_B,
	CF_SPIKE_C,
	SCR_G_UP,
	SCR_G_DOWN,
	SCR_G_ANY,
	SCR_G_TICK,
	SCR_B_UP,
	SCR_B_DOWN,
	SCR_B_ANY,
	SCR_B_TICK,
	SCR_G_ZONE,
	SCR_B_ZONE,
	EU_ZONE,
	CROSS_G_TICK,
	CROSS_B_TICK
};

class Note {
public:
	Note(double milli, int ty, double length, bool ev = false);
	void click();
	void tick(double time);
	double getMilli() const;
	double getLength() const;
	int getType() const;
	bool getHit() const;
	bool getTouched() const;
	void setTouched(bool value);
	bool getDead() const;
	bool getIsEvent() const;
	int getLanMod() const;
	void setLanMod(int i);

	double hitWindow = 0.20;

protected:
private:
	double m_milli;
	double m_length;

	double m_hit_window;
	bool m_isEvent;
	bool m_hittable = false;
	bool m_touched = false;
	bool m_dead = false;
	bool m_firstClick = true;
	int m_type;
	int m_lan_mod = -1;
};