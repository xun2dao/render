#include"EventDispatcher.h"

EventDispatcher::EventDispatcher(){
    std::thread t(&EventDispatcher::Handle, this);
    t.detach();
}

void EventDispatcher::Handle(){
    while(true){
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