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