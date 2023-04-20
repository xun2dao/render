#pragma once
#include<queue>
#include<thread>
#include<mutex>
#include<unordered_map>
#include<functional>
#include"Event/Event.h"


class EventCompare{
public:
    bool operator()(Event* e1, Event* e2){
        return (*e1) < (*e2);
    }
};

class EventDispatcher{
public:
    EventDispatcher();
    void PushEvent(Event* e){
        m_lock.lock();
        m_events.emplace(e);
        m_lock.unlock();
    }

    void Handle();
    void Register(const EventType& type, const std::function<void(const Event*)>& func){
        m_event_handlers[type] = func;
    }
    ~EventDispatcher(){
        m_running = false;
        m_thread.join();
    }
private:
    std::priority_queue<Event*,std::vector<Event*>, EventCompare> m_events;
    std::mutex m_lock;
    std::unordered_map<EventType, std::function<void(const Event*)>> m_event_handlers;
    std::thread m_thread;
    bool m_running = true;
};