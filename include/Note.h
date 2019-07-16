#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <SFML/System.hpp>

enum events {TAP_G,TAP_R,TAP_B,CROSS_L,CROSS_R,CROSS_C};

class Note {
public:
    Note();
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
    bool m_active = true;
    float m_milli;
    int m_type;
};

#endif // NOTE_H
