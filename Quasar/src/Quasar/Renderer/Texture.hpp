#pragma once

#include "Quasar/Core/Core.hpp"

#include <string>
#include <cstdint>

namespace Quasar
{
    
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getheight() const = 0;

        virtual void bind(uint32_t slot = 0) const = 0;  
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> create(const std::string &path);
    };



} // namespace Quasar