#include"EventDispatcher.h"

EventDispatcher::EventDispatcher(){
    m_thread = std::thread(&EventDispatcher::Handle, this);
}

void EventDispatcher::Handle(){
    while(m_running){
        m_lock.lock();
        if(m_events.empty()){
            m_lock.unlock();
            continue;
        }
        const Event* e = m_events.top();
        m_events.pop();
        m_lock.unlock();

        auto func = m_event_handlers[e->Type()];
        func(e);
        delete e;
    }
}