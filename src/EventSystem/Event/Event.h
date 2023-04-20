#pragma once
#include<string>
#define BITS(x) (1<<x)

// 最多有32个属性
namespace EventProperty{
    static unsigned int UNKNOWN = 0;
    static unsigned int INTPUT = BITS(0);
    static unsigned int OUTPUT = BITS(1);
}

enum class EventType{
    UNKNOWN,
    MouseButton,
    KeyPress,
    MouseScroll,
    MouseCursorPos
};

#define EVENT_TYPE(x) \
    virtual EventType Type() const override{\
        return EventType::x;\
    }

class Event{
public:
    virtual std::string ToString() const = 0;
    bool IsMatch(const unsigned int & property) {return (m_property&property);}
    
    bool operator<(const Event& other){
        return m_priority < other.m_priority;
    }
    virtual EventType Type() const{
        return EventType::UNKNOWN;
    }
    virtual ~Event(){}
    
protected:
    unsigned int m_property = EventProperty::UNKNOWN;
    int m_priority = INT_MAX;
};