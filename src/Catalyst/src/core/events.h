#pragma once

#include "internal.h"

#include "managment/manager.h"

namespace Catalyst
{

    typedef unsigned long      EventType;
    typedef unsigned long long EventID;

    class ListenerManager;

    /**
     * @class IBaseEvent
     *
     * @brief A tracker class that is used to point at an event
     */
    class IBaseEvent
    {

    public:
        const EventID ID = createID();

    public:
        constexpr const EventID& getID() const;

        virtual constexpr const EventType& getType() const = 0;

    protected:
        static const EventType createType();

    private:
        static const EventID   createID();

    };

    /**
     * @class IEvent
     * 
     * @brief A tracker class that sorts event types and assigned ids at compile time
     */
    template<typename T>
    class IEvent : public IBaseEvent
    {

    public:
        inline static const auto type = createType();

    public:
        virtual ~IEvent() = default;

        virtual constexpr const EventType& getType() const override
        {
            return type;
        }

    protected:
        IEvent() = default;

    };


    class EventManager : public IManager<EventType, std::vector<std::shared_ptr<IBaseEvent>>>
    {

        friend class ListenerManager;

    public:
        template<typename Event, typename... Args>
        static constexpr void construct(Args&&... args)
        {

            static_assert(std::constructible_from<Event, Args...>, "[CATALYST] [EVENT] Cannot construct event! Have you provided a constructor? Are the arguments correct?");

            std::shared_ptr<Event> event = std::make_shared<Event, Args...>(std::move(args...));
            s_Data[Event::type].push_back(std::static_pointer_cast<IBaseEvent>(event));
        }

    private:
        static std::vector<std::shared_ptr<IBaseEvent>>& request(const EventType& type);
        static const size_t size();

    };


    /**
    * 
    * @class IBaseListener
    * 
     * @brief Storable class that points at the location of a callable listener function.
     */
    class IBaseListener
    {

    public:
        inline IBaseListener() = default;
        inline virtual ~IBaseListener() = default;

        virtual bool onEvent(std::shared_ptr<IBaseEvent>) = 0;
        virtual constexpr const EventType getType() const = 0;

    };

    /**
     * @class ListenerFunction
     * 
     * @brief Stores a callable function in a manner which can be stored for later use.
     * 
     * @tparam Class - IListener class
     * 
     * Note: There is no need to worry about the pointer as this object is tied to the
     * lifetime of the pointer.
     */
    template<typename Class, typename Event>
    class ListenerStorage : public IBaseListener
    {
        typedef bool(Class::*Function)(Event&&);

    public:
        inline explicit ListenerStorage(Class* store, Function function)
            : m_Class(store), m_Function(function)
        {
            ListenerManager::addListener(this);
        }
        inline ~ListenerStorage()
        {
            ListenerManager::removeListener(this);
        }

    private:
        inline virtual bool onEvent(std::shared_ptr<IBaseEvent> event) override
        {
#ifdef CATALYST_DEBUG
            if (event->getType() == Event::type && m_Class)
            {
                return (m_Class->*m_Function)(std::move(*std::static_pointer_cast<Event>(event)));
            }
            else
            {
                Engine::raiseError({ Level::Error, "CatalystResult::Executing_Unreachable_Code", CatalystResult::Executing_Unreachable_Code, __FUNCTION__ });
                return false;
            }
#else
            return (m_Class->*m_Function)(std::move(*std::static_pointer_cast<Event>(event)));
#endif
        }

        inline virtual constexpr const EventType getType() const override
        {
            return Event::type;
        }

    private:
        Class* m_Class;
        Function m_Function;

    };

    template<typename... Events>
    class IListener;

    template<typename Event>
    class IListener<Event>
    {
        static_assert(std::is_base_of_v<IBaseEvent, Event>, "[CATALYST] [LISTENER] Class provided does not implement/extend IEvent!");

    private:
        using Storage = ListenerStorage<IListener<Event>, Event>;

    public:
        inline IListener() = default;
        inline virtual ~IListener() = default;

        virtual bool onEvent(Event&&) = 0;

    private:
        Storage m_Storage = Storage(this, &IListener<Event>::onEvent);

    };

    template<typename Event, typename... Events>
    class IListener<Event, Events...> : IListener<Events...>
    {
        static_assert(std::is_base_of_v<IBaseEvent, Event>, "[CATALYST] [LISTENER] Class provided does not implement/extend IEvent!");

    private:
        using Storage = ListenerStorage<IListener<Event, Events...>, Event>;

    public:
        inline IListener() = default;
        virtual ~IListener() = default;

        virtual bool onEvent(Event&&) = 0;

    private:
        Storage m_Storage = Storage(this, &IListener<Event, Events...>::onEvent);

    };

    class ListenerManager : public IManager<EventType, std::vector<IBaseListener*>>
    {

        friend class Engine;
        template<typename Class, typename Event>
        friend class ListenerStorage;

    private:
        static void update();

    protected:
        static void addListener(IBaseListener* listener);
        static void removeListener(IBaseListener* listener);
    };

}
