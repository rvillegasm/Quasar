#include "Scene.hpp"

#include "Quasar/Renderer/Renderer2D.hpp"
#include "Quasar/Renderer/Camera.hpp"
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
        // Render 2D
        Camera *mainCamera = nullptr;
        glm::mat4 *cameraTransform = nullptr;
        
        auto tcView = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : tcView)
        {
            auto [transformComponent, cameraComponent] = tcView.get<TransformComponent, CameraComponent>(entity);

            if (cameraComponent.primary)
            {
                mainCamera = &cameraComponent.camera;
                cameraTransform = &transformComponent.transform;
                break;
            }
        }

        if (mainCamera)
        {
            Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);

            auto tsGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : tsGroup)
            {
                auto [transform, sprite] = tsGroup.get<TransformComponent, SpriteRendererComponent>(entity);
                
                Renderer2D::drawQuad(transform, sprite.color);
            }

            Renderer2D::endScene();
        }

    }

} // namespace Quasar
