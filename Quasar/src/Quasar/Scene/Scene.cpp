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

    void Scene::destroyEntity(Entity entity) 
    {
        m_Registry.destroy(entity);
    }
    
    void Scene::onUpdateRuntime(Timestep ts)
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
        glm::mat4 cameraTransform;
        
        auto tcView = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : tcView)
        {
            auto [transformComponent, cameraComponent] = tcView.get<TransformComponent, CameraComponent>(entity);

            if (cameraComponent.primary)
            {
                mainCamera = &cameraComponent.camera;
                cameraTransform = transformComponent.getTransform();
                break;
            }
        }

        if (mainCamera)
        {
            Renderer2D::beginScene(*mainCamera, cameraTransform);

            auto tsGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : tsGroup)
            {
                auto [transformComponent, spriteComponent] = tsGroup.get<TransformComponent, SpriteRendererComponent>(entity);
                
                Renderer2D::drawSprite(transformComponent.getTransform(), spriteComponent, (int)entity);
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onUpdateEditor(Timestep ts, EditorCamera &camera)
    {
        Renderer2D::beginScene(camera);

        auto tsGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : tsGroup)
        {
            auto [transformComponent, spriteComponent] = tsGroup.get<TransformComponent, SpriteRendererComponent>(entity);
            
            Renderer2D::drawSprite(transformComponent.getTransform(), spriteComponent, (int)entity);
        }

        Renderer2D::endScene();
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
    
    Entity Scene::getPrimaryCameraEntity() 
    {
        auto cView = m_Registry.view<CameraComponent>();
        for (auto entity : cView)
        {
            const auto &cameraComponent = cView.get<CameraComponent>(entity);
            if (cameraComponent.primary)
            {
                return Entity{ entity, this };
            }
        }
        return {};
    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T &component)
    { // There should always be a template specialization
    }

    template<>
    void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent &component)
    { // Nothing for now; Maybe do something in the future
    }

    template<>
    void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent &component)
    { // Nothing for now; Maybe do something in the future
    }

    template<>
    void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent &component)
    { // Nothing for now; Maybe do something in the future
    }

    template<>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent &component)
    {
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
        {
            component.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
        }
    }

    template<>
    void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent &component)
    { // Nothing for now; Maybe do something in the future
    }

    // TODO: add onComponentRemoved as well (for example for destraying script instances)

} // namespace Quasar
