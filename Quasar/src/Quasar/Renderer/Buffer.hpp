#pragma once

#include <Quasar/Core/Core.hpp>

#include <cstdint>
#include <vector>
#include <string>

namespace Quasar
{

    enum class ShaderDataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool,
    };

    struct BufferElement
    {
        ShaderDataType type;
        std::string name;
        uint32_t size;
        size_t offset;
        bool normalized;

        BufferElement() = default;
        BufferElement(
            ShaderDataType type,
            const std::string &name,
            bool normalized = false
        );

        uint32_t getComponentCount() const;
    };

    class BufferLayout
    {
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;

        void calculateOffsetsAndStride();

    public:
        BufferLayout() {}
        BufferLayout(const std::initializer_list<BufferElement> &elements);

        inline uint32_t getStride() const { return m_Stride; }
        inline const std::vector<BufferElement> &getElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return  m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return  m_Elements.end(); }
    };
    
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const BufferLayout &getLayout() const = 0;
        virtual void setLayout(const BufferLayout &layout) = 0;
        
        static Ref<VertexBuffer> create(float *vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;
        
        static Ref<IndexBuffer> create(uint32_t *indices, uint32_t count);
    };

} // namespace Quasar
