#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <SFML/System.hpp>
#include <iostream>

enum note {TAP_G,TAP_R,TAP_B,
            CROSS_L,CROSS_R,CROSS_C,
            SCR_G_UP,SCR_G_DOWN,SCR_G_ANY,
            SCR_B_UP,SCR_B_DOWN,SCR_B_ANY,
            SCR_G_START,SCR_G_END,SCR_B_START,SCR_B_END};

class Note {
public:
    Note(float milli,int ty);
    virtual ~Note();
    void click(float time);
    void tick(float time);
    float getMilli();
    int getType();
    bool getActive();
    void destroy();
protected:

private:
    float m_hit_window;
    bool m_active = true;
    float m_milli;
    int m_type;
};

#endif // NOTE_H
