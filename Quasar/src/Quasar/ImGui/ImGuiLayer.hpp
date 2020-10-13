#pragma once

#include "Quasar/Layer.hpp"

namespace Quasar
{
    
    class ImGuiLayer : public Layer
    {
    private:
        float m_Time = 0.0f;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event &event);
    };

} // namespace Quasar
