#pragma once

#include <unordered_map>

#include "factory.h"

namespace Catalyst
{

    template<typename Key, typename Value>
    class IManager
    {
      
        using factory = IFactory<Value>;

    public:
        template<typename... Args>
        static constexpr Value construct(Key key, Args&&... args)
        {
            return (s_Data[key] = factory::construct(args...));
        }

        static constexpr Value request(Key key)
        {
            if (s_Data.find(key) != s_Data.end())
            {
                return s_Data[key];
            }
            else
            {
                return IManager<Key, Value>::construct(key);
            }
        }

    protected:
        inline static std::unordered_map<Key, Value> s_Data;

    };
    
    template<typename Key, typename Value>
    class IManagerShared
    {
      
        using factory = IFactoryShared<Value>;

    public:
        template<typename... Args>
        static constexpr auto construct(Key key, Args&&... args)
        {
            return (s_Data[key] = factory::construct(args...));
        }
        template<typename... Args>
        static constexpr auto request(Key key, Args&&... args)
        {
            if (s_Data.find(key) != s_Data.end())
            {
                return s_Data[key];
            }
            else
            {
                return IManagerShared<Key, Value>::construct(key, args...);
            }
        }

    protected:
        inline static std::unordered_map<Key, std::shared_ptr<Value>> s_Data;

    };

}
