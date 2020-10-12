#pragma once

#include "Quasar/Events/Event.hpp"

#include <string>

namespace Quasar
{

    class Layer
    {
    protected:
        // Name should only be there for debug purposes
        std::string m_DebugName;

    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onEvent(Event &event) {}

        inline const std::string &getName() const { return m_DebugName; }
    };

} // namespace Quasar