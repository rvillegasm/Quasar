#include "Buffer.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Quasar
{

    static uint32_t shaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3:   return 4 * 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4 * 4;
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 4 * 2;
            case ShaderDataType::Int3:   return 4 * 3;
            case ShaderDataType::Int4:   return 4 * 4;
            case ShaderDataType::Bool:   return 1;

            default:
                QS_CORE_ASSERT(false, "Unkown ShaderDataType!");
                return 0;
        }
    }

    // --------------------------
    // ----- BUFFER ELEMENT -----
    // --------------------------

    BufferElement::BufferElement(
        ShaderDataType type,
        const std::string &name,
        bool normalized /*= false*/
    ) : type(type), name(name), size(shaderDataTypeSize(type)), offset(0)
    {
    }

    uint32_t BufferElement::getComponentCount() const
    {
        switch (type)
        {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;

            default:
                QS_CORE_ASSERT(false, "Unkown ShaderDataType!");
                return 0;
        }
    }

    // --------------------------
    // ----- BUFFER LAYOUT -----
    // --------------------------

    BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &elements)
        : m_Elements(elements)
    {
        calculateOffsetsAndStride();
    }

    void BufferLayout::calculateOffsetsAndStride()
    {
        size_t offset = 0;
        m_Stride = 0;
        for (auto &element : m_Elements)
        {
            element.offset = offset;
            offset += element.size;
            m_Stride += element.size;
        }
    }

    // -------------------------
    // ----- VERTEX BUFFER -----
    // -------------------------

    Ref<VertexBuffer> VertexBuffer::create(uint32_t size) 
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(size);

            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }

    Ref<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(vertices, size);

            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }

    // ------------------------
    // ----- INDEX BUFFER -----
    // ------------------------

    Ref<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t count)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return createRef<OpenGLIndexBuffer>(indices, count);

            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }

} // namespace Quasar
