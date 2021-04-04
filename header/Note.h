// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <iostream>

enum NoteTypes {
	TAP_G, //inside note vector
	TAP_R, //inside note vector
	TAP_B, //inside note vector
	CROSS_G, //inside crossfade vector
	CROSS_B, //inside crossfade vector
	CROSS_C, //inside crossfade vector
	CF_SPIKE_G, //inside note vector
	CF_SPIKE_B, //inside note vector
	CF_SPIKE_C, //inside note vector
	SCR_G_UP, //inside note vector
	SCR_G_DOWN, //inside note vector
	SCR_G_ANY, //inside note vector
	SCR_G_TICK, //inside note vector
	SCR_B_UP, //inside note vector
	SCR_B_DOWN, //inside note vector
	SCR_B_ANY, //inside note vector
	SCR_B_TICK, //inside note vector
	SCR_G_ZONE, //inside event vector
	SCR_B_ZONE, //inside event vector
	EU_ZONE, //inside event vector
	CROSS_G_TICK, //inside crossfade vector
	CROSS_B_TICK, //inside crossfade vector
	TAP_G_HOLD_TICK, //inside note vector
	TAP_R_HOLD_TICK, //inside note vector
	TAP_B_HOLD_TICK, //inside note vector
	FS_CROSS_BASE, //inside event vector
	FS_SAMPLES //inside event vector
};

enum FSGNoteTypes {
	FSG_TAP_G,
	FSG_TAP_B,
	FSG_TAP_R,
	FSG_SCR_G_UP,
	FSG_SCR_B_UP,
	FSG_SCR_G_DOWN,
	FSG_SCR_B_DOWN,
	FSG_SCR_G_ANY,
	FSG_SCR_B_ANY,
	FSG_CROSS_B,
	FSG_CROSS_C,
	FSG_CROSS_G,
	FSG_FX_G,
	FSG_FX_B,
	FSG_FX_R,
	FSG_EUPHORIA,
	FSG_FS_SAMPLES,
	FSG_FS_CROSS,
	FSG_SCR_G_ZONE = 20,
	FSG_SCR_B_ZONE = 21,
	FSG_FX_ALL,
	FSG_CROSS_FORCE_CENTER,
	FSG_BATTLE_TAG = 26,
	FSG_CF_SPIKE_G,
	FSG_CF_SPIKE_B,
	FSG_CF_SPIKE_C,
	FSG_MEGAMIX_TRANSITION,
	FSG_FS_CROSS_G_MARKER,
	FSG_FS_CROSS_B_MARKER,
	FSG_FX_TYPE_FILTER = 0x05ffffff,
	FSG_FX_TYPE_BEATROLL,
	FSG_FX_TYPE_BITREDUCTION,
	FSG_FX_TYPE_WAHWAH,
	FSG_FX_TYPE_RINGMOD,
	FSG_FX_TYPE_STUTTER,
	FSG_FX_TYPE_FLANGER,
	FSG_FX_TYPE_ROBOT,
	FSG_FX_TYPE_BEATROLLAUTO,
	FSG_FX_TYPE_DELAY = 0x06000009,
	FSG_BPM_FAKE_DISTANCE = 0x0b000001,
	FSG_BPM,
	FSG_REWIND = 0x09ffffff
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