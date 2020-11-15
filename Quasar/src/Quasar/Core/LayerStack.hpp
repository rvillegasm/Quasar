#pragma once

#include "Quasar/Core/Layer.hpp"

#include <vector>

namespace Quasar
{

    class LayerStack
    {
    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    
    public:
        LayerStack() = default;
        ~LayerStack();

        void pushLayer(Layer *layer);
        void pushOverlay(Layer *overlay);
        void popLayer(Layer *layer);
        void popOverlay(Layer *overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    };

}