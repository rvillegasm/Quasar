#include "LayerStack.hpp"

#include <algorithm>

namespace Quasar
{

    LayerStack::LayerStack()
    {
    }

    LayerStack::~LayerStack()
    {
        // the LayerStack own the Layers, and they get destroyed when
        // the application terminates (at least for now)
        for (Layer *layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer *layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::pushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Layer *overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }

} // namespace Quasar