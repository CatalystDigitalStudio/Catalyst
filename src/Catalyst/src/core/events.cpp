#include "pch.h"
#include "Events.h"

namespace Catalyst
{
    constexpr const EventID& IBaseEvent::getID() const
    {
        return ID;
    }
    const EventType IBaseEvent::createType()
    {
        static std::atomic<EventType> s_Index = 0;

        s_Index.store(s_Index.load() + 1);

        return s_Index.load();
    }

    const EventID IBaseEvent::createID()
    {
        static std::atomic<EventType> s_Index = 0;

        s_Index.store(s_Index.load() + 1);

        return s_Index.load();
    }

    void ListenerManager::update()
    {
        if (EventManager::size())
        {
            for (auto& listeners : s_Data)
            {
                for (auto& event : EventManager::request(listeners.first))
                {
                    for (auto& listener : listeners.second)
                    {
                        if (listener->onEvent(event))
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    void ListenerManager::addListener(IBaseListener* listener)
    {
        s_Data[listener->getType()].push_back(listener);
    }

    void ListenerManager::removeListener(IBaseListener* listener)
    {
        auto& listeners = s_Data[listener->getType()];
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener));
    }

    std::vector<std::shared_ptr<IBaseEvent>>& EventManager::request(const EventType& type)
    {
        return s_Data[type];
    }

    const size_t EventManager::size()
    {
        return s_Data.size();
    }

}
