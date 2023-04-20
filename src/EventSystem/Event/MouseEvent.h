#pragma once
#include"Event.h"


class MouseButtonEvent : public Event{
public:
    MouseButtonEvent(int button, int priority = 0):m_button(button){
        m_priority = priority;
    }
    std::string ToString() const override;

    EVENT_TYPE(MouseButton)

    virtual ~MouseButtonEvent(){}
private:
    int m_button;
};


class MouseScrollEvent : public Event{
public:
    MouseScrollEvent(double x, double y, int priority = 0):m_x(x), m_y(y){
        m_priority = 0;
    }
    std::string ToString() const override;
    EVENT_TYPE(MouseScroll);

    virtual ~MouseScrollEvent(){}
private:
    double m_x;
    double m_y;
};

class MouseCursorPos : public Event{
public:
    MouseCursorPos(double x, double y, int priority = 0):m_x(x), m_y(y){
        m_priority = 0;
    }
    std::string ToString() const override;
    EVENT_TYPE(MouseCursorPos);

    virtual ~MouseCursorPos(){}
private:
    double m_x;
    double m_y;
};