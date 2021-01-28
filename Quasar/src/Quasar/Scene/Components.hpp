#pragma once

#include "Quasar/Scene/SceneCamera.hpp"
#include "Quasar/Scene/ScriptableEntity.hpp"

#include <glm/glm.hpp>

#include <string>

namespace Quasar
{

    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        TagComponent(const std::string &tag)
            : tag(tag)
        {
        }
    };
    
    struct TransformComponent
    {
        glm::mat4 transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        TransformComponent(const glm::mat4 &transform)
            : transform(transform)
        {
        }

        operator glm::mat4 &() { return transform; }
        operator const glm::mat4 &() const { return transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent &) = default;
        SpriteRendererComponent(const glm::vec4 &color)
            : color(color)
        {
        }
    };

    struct CameraComponent
    {
        SceneCamera camera;
        bool primary = true; // TODO: think about moving to scene
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent &) = default;
    };

    struct NativeScriptComponent
    {
        ScriptableEntity *instance = nullptr;

        ScriptableEntity *(*instantiateScript)();
        void (*destroyScript)(NativeScriptComponent *);

        template<typename T>
        void bind()
        {
            instantiateScript = []() { return static_cast<ScriptableEntity *>(new T()); };
            destroyScript = [](NativeScriptComponent *nsc) { delete nsc->instance; nsc->instance = nullptr; };
        }
    };

} // namespace Quasar
