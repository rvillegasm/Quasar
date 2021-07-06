#pragma once

#include "Quasar/Core/Layer.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"
#include "Quasar/Events/KeyEvent.hpp"
#include "Quasar/Events/MouseEvent.hpp"

namespace Quasar
{
    
    class ImGuiLayer : public Layer
    {
    private:
        bool m_BlockEvents = true;

    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void onAttach() override;
        void onDetach() override;
        void onEvent(Event &e) override;

        void begin();
        void end();

        void blockEvents(bool block) { m_BlockEvents = block; }

        void setDarkThemeColors();
    };

} // namespace Quasar
