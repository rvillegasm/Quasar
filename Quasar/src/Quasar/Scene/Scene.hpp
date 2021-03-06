#pragma once

#include "Quasar/Core/Timestep.hpp"
#include "Quasar/Renderer/EditorCamera.hpp"

#include <entt/entity/registry.hpp>

#include <string>

namespace Quasar
{
    
    class Entity;

    class Scene
    {
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;

        template<typename T>
        void onComponentAdded(Entity entity, T &component);

    public:
        Scene();

        Entity createEntity(const std::string &name = std::string());
        void destroyEntity(Entity entity);

        void onUpdateRuntime(Timestep ts);
        void onUpdateEditor(Timestep ts, EditorCamera &camera);
        void onViewportResize(uint32_t width, uint32_t height);

        Entity getPrimaryCameraEntity();
    };

} // namespace Quasar
