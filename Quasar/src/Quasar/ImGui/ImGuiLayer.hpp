#pragma once

#include "Quasar/Layer.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"
#include "Quasar/Events/KeyEvent.hpp"
#include "Quasar/Events/MouseEvent.hpp"

namespace Quasar
{
    
    class ImGuiLayer : public Layer
    {
    private:
        float m_Time = 0.0f;

        bool onMouseButtonPressedEvent(MouseButtonPressedEvent &e);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent &e);
        bool onMouseMovedEvent(MouseMovedEvent &e);
        bool onMouseScrolledEvent(MouseScrolledEvent &e);
        bool onKeyPressedEvent(KeyPressedEvent &e);
        bool onKeyReleasedEvent(KeyReleasedEvent &e);
        bool onKeyTypedEvent(KeyTypedEvent &e);
        bool onWindowResizeEvent(WindowResizeEvent &e);

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event &event);
    };

} // namespace Quasar
