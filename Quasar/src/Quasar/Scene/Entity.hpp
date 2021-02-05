#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Scene/Scene.hpp"

#include <entt/entity/registry.hpp>

namespace Quasar
{
    
    class Entity
    {
    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene *m_Scene = nullptr; // Should be a weak ref to a scene later

    public:
        Entity() = default;
        Entity(const Entity &other) = default;
        Entity(entt::entity handle, Scene *scene);

        template<typename T, typename ...Args>
        T &addComponent(Args &&...args)
        {
            QS_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
            T &component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->onComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T &getComponent()
        {
            QS_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool hasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void removeComponent()
        {
            QS_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity &other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity &other) const { return !(*this == other); }
    };

} // namespace Quasar
