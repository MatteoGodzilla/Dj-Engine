#pragma once
#include <iostream>

enum note {
	TAP_G,TAP_R,TAP_B,
	CROSS_G, CROSS_B, CROSS_C,
	CF_SPIKE_G, CF_SPIKE_B, CF_SPIKE_C,
	SCR_G_UP, SCR_G_DOWN, SCR_G_ANY,
	SCR_B_UP,SCR_B_DOWN,SCR_B_ANY,
	SCR_G_ZONE,SCR_B_ZONE,EU_ZONE
};

class Note {
public:
	Note(double milli, int ty, double length, bool ev = false);
    virtual ~Note();
	void click(double time);
    void tick(double time);
    double getMilli();
	double getLength();
    int getType();
    bool getHit();
    bool getTouched();
	void setTouched(bool value);
    bool getDead();
    bool getIsEvent();
    int getLanMod();
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
    int m_type;
    int m_lan_mod = -1;
};