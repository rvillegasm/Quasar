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

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onImGuiRender() override;

        void begin();
        void end();
    };

} // namespace Quasar
