#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "eventhandler.hpp"
#include <list>
#include <mutex>
#include <queue>
#include <map>

class EventManager {
public:
    EventManager() = default;
    ~EventManager() = default;

    void registerHandler(EventType eventType, EventHandler* handler);

    void deregisterHandler(EventType eventType, EventHandler* handler);

    void raise(Event event);

    void processEvents();

    void replayEvents(Game& game);

    bool isRecording = false, isReplaying = false;
    std::priority_queue<Event, std::vector<Event>, std::less<>> replayEventQueue;

private:
    std::map<EventType, std::list<EventHandler*>> handlers;
    std::priority_queue<Event, std::vector<Event>, std::less<>> eventQueue;
    std::mutex mutex;
};

#endif
