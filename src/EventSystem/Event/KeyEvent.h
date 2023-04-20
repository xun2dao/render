#pragma once
#include"Event.h"
#include<sstream>
class KeyPressEvent : public Event{
public:

KeyPressEvent(int keycode, int repeat) : m_keycode(keycode), m_repeat(repeat){}

std::string ToString() const override{
    std::stringstream ss;
    ss << "KeyPressEvent [code : repeat]: ["<<m_keycode<<","<<m_repeat<<"]";
    return ss.str();
}
EVENT_TYPE(KeyPress)
~KeyPressEvent(){}

int GetKey() const {return m_keycode;}
private:
    int m_keycode;
    int m_repeat;
};