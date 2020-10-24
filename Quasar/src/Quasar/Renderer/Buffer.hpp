#pragma once

#include <cstdint>

namespace Quasar
{
    
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        
        static VertexBuffer *create(float *vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;
        
        static IndexBuffer *create(uint32_t *indices, uint32_t count);
    };

} // namespace Quasar
