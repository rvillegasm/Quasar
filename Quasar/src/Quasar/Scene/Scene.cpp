#include "Scene.hpp"

#include "Quasar/Renderer/Renderer2D.hpp"
#include "Quasar/Scene/Components.hpp"
#include "Quasar/Scene/Entity.hpp"

namespace Quasar
{
    Scene::Scene() 
    {   
    }
    
    Entity Scene::createEntity(const std::string &name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.addComponent<TransformComponent>();
        auto &tagComponent = entity.addComponent<TagComponent>();
        tagComponent.tag = name.empty() ? "Entity" : name;

        return entity;
    }
    
    void Scene::onUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            
            Renderer2D::drawQuad(transform, sprite.color);
        }
    }

} // namespace Quasar
