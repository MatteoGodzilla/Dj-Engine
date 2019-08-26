#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <iostream>

enum note {TAP_G,TAP_R,TAP_B,
            CROSS_L,CROSS_R,CROSS_C,
            SCR_G_UP,SCR_G_DOWN,SCR_G_ANY,
            SCR_B_UP,SCR_B_DOWN,SCR_B_ANY,
            SCR_G_START,SCR_G_END,SCR_B_START,SCR_B_END};

class Note {
public:
    Note(double milli,int ty,bool ev = false);
    virtual ~Note();
    void click(float time);
    void tick(float time);
    float getMilli();
    int getType();
    bool getRender();
    bool getHit();
    bool getTouched();
    bool getDead();
    bool getIsEvent();
    int getLanMod();
    void setLanMod(int i);
protected:

private:
    float m_hit_window;
    bool m_is_event;
    bool m_render = true;
    bool m_hittable = false;
    bool m_touched = false;
    bool m_dead = false;
    double m_milli;
    int m_type;
    int m_lan_mod = -1;
};

#endif // NOTE_H
