#include "Application.hpp"

#include "Quasar/Log.hpp"
#include "Quasar/Input.hpp"

#include <glad/glad.h>

namespace Quasar
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;

    static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        
            default:
                QS_CORE_ASSERT(false, "Unkown ShaderDataType!");
                return 0;
        }
    }

    Application::Application()
    {
        QS_CORE_ASSERT(!s_Instance, "An Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::create());
        m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer();
        pushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);        


        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        m_VertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" },
            };

            m_VertexBuffer->setLayout(layout);
        }


        uint32_t index = 0;
        const auto &layout = m_VertexBuffer->getLayout();
        for (const auto &element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.getComponentCount(),
                shaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.getStride(),
                (const void *)element.offset
            );
            index++;
        }


        unsigned int indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        const std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        const std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
    }

    Application::~Application() 
    {
    }

    void Application::pushLayer(Layer *layer)
    {
        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer *overlay)
    {
        m_LayerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->onEvent(e);
            if (e.isHandled())
            {
                break;
            }
        }
    }

    void Application::run()
    {
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer *layer : m_LayerStack)
            {
                layer->onUpdate();
            }

            m_ImGuiLayer->begin();
            for (Layer *layer : m_LayerStack)
            {
                layer->onImGuiRender();
            }
            m_ImGuiLayer->end();

            m_Window->onUpdate();
        }
    }

    bool Application::onWindowClosed(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

} // namespace Quasar