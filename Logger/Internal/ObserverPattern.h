#ifndef __OBSERVER_PATTERN__
#define __OBSERVER_PATTERN__

#include <vector>
#include <unordered_map>

template <typename T, typename U>
class Subject;

//  Observer class, EventType describes at what type of events will this observer look
//  and EventDataType describes the data type received from the subject
template <typename EventType, typename EventDataType>
class Observer
{
public:
    virtual ~Observer() { }
    virtual void updateObserver(const Subject<EventType, EventDataType>* subject) = 0;
};

//  Helper struct to create hash
template <typename T>
struct SimpleHash
{
    std::size_t operator()(const T& t) const
    {
        return sizeof(t);
    }
};

//  Subject class, whenever an event of type EventType occurs, it broadcasts it's
//  event data to all interested observers
template <typename EventType, typename EventDataType>
class Subject
{
    using EventObserver = Observer<EventType, EventDataType>;

private:
    std::unordered_map<EventType, std::vector<EventObserver*>, SimpleHash<EventType> > registryMap;

    EventDataType eventData;
    EventType eventType;

public:
    Subject() { }
    virtual ~Subject() { }

    inline void attach(const EventType& event, EventObserver* observer)
    {
        registryMap[event].push_back(observer);
    }

    inline void detach(const EventType& event, EventObserver* observer)
    {
        std::vector<EventObserver*>& v = registryMap[event];
        for (int i = v.size() - 1; i >= 0; --i) {
            if (v[i] == observer) {
                v.erase(v.begin() + i);

                return;
            }
        }
    }

    inline void detachFromAllEvents(EventObserver* observer)
    {
        for (auto& it : registryMap)
            detach(it.first, observer);
    }

    inline void notify(const EventType& event)
    {
        auto it = registryMap.find(event);
        if (it != registryMap.end()) {
            eventType = event;

            for (auto& observer : it->second)
                observer->updateObserver(this);
        }
    }

    inline void setEventData(const EventDataType& eventData) { this->eventData = eventData; }
    inline const EventDataType& getEventData() const { return this->eventData; }
    inline const EventType& getEventType() const { return this->eventType; }
};

using SimpleSubject = Subject<int, int>;
using SimpleObserver = Observer<int, int>;

#endif // __OBSERVER_PATTERN__
