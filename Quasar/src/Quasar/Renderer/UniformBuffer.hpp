#pragma once

#include "Quasar/Core/Base.hpp"

namespace Quasar
{

    class UniformBuffer
    {
    private:


    public:
        virtual ~UniformBuffer() = default;
        virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) = 0;

        static Ref<UniformBuffer> create(uint32_t size, uint32_t binding);
    };

}