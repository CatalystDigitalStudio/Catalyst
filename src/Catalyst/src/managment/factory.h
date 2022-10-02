#pragma once

#include <memory>

namespace Catalyst
{

    template<typename Product>
    class IFactory
    {

    public:
        template<typename... Args>
        static constexpr Product construct(Args&&... args)
        {
            return std::move(Product(args...));
        }
    };

    template<typename Product>
    class IFactoryShared
    {

    public:
        template<typename... Args>
        static constexpr std::shared_ptr<Product> construct(Args&&... args)
        {
            std::shared_ptr<Product> result = std::make_shared<Product>();
            result.reset(new Product(args...));
            return result;
        }
    };
}
