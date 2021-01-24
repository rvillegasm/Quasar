#pragma once

#include "Quasar/Core/Timestep.hpp"

#include <entt/entity/registry.hpp>

#include <string>

namespace Quasar
{
    
    class Entity;

    class Scene
    {
        friend class Entity;

    private:
        entt::registry m_Registry;

    public:
        Scene();

        Entity createEntity(const std::string &name = std::string());

        void onUpdate(Timestep ts);
    };

} // namespace Quasar
