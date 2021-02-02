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
        // Update Native Scripts
        m_Registry.view<NativeScriptComponent>().each([this, ts](auto entity, NativeScriptComponent &nsc)
        {
            // TODO: Move to Scene::onScenePlay
            if (!nsc.instance)
            {
                nsc.instance = nsc.instantiateScript();
                nsc.instance->m_Entity = Entity{ entity, this };
                nsc.instance->onCreate();
            }
            
            nsc.instance->onUpdate(ts);
        });

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
            Renderer2D::beginScene(*mainCamera, *cameraTransform);

            auto tsGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : tsGroup)
            {
                auto [transform, sprite] = tsGroup.get<TransformComponent, SpriteRendererComponent>(entity);
                
                Renderer2D::drawQuad(transform, sprite.color);
            }

            Renderer2D::endScene();
        }

    }
    
    void Scene::onViewportResize(uint32_t width, uint32_t height) 
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize our non-fixed aspect ratio cameras
        auto cView = m_Registry.view<CameraComponent>();
        for (auto entity : cView)
        {
            auto &cameraComponent = cView.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio)
            {
                cameraComponent.camera.setViewportSize(width, height);
            }
        }
    }

} // namespace Quasar
