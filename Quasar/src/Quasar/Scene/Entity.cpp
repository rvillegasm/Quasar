#include "Entity.hpp"

namespace Quasar
{

    Entity::Entity(entt::entity handle, Scene *scene) 
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

} // namespace Quasar
