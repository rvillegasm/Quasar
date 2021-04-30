#pragma once

#include "Quasar/Scene/SceneCamera.hpp"
#include "Quasar/Scene/ScriptableEntity.hpp"
#include "Quasar/System/DllLoader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace Quasar
{

    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent &) = default;
        explicit TagComponent(const std::string &tag)
            : tag(tag)
        {
        }
    };

    struct TransformComponent
    {
        glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent &) = default;
        explicit TransformComponent(const glm::vec3 &translation)
            : translation(translation)
        {
        }

        glm::mat4 getTransform() const
        {
            glm::mat4 rotationMat = glm::toMat4(glm::quat(rotation));

            return glm::translate(glm::mat4(1.0f), translation)
                * rotationMat
                * glm::scale(glm::mat4(1.0f), scale);
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent &) = default;
        explicit SpriteRendererComponent(const glm::vec4 &color)
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
        Ref<ScriptableEntity> instance = nullptr;
        Scope<DllLoader<ScriptableEntity>> dllLoader = nullptr;

        std::function<Ref<ScriptableEntity>()> instantiateScript; // using instead of func pointer because of lambda capture in bindDll

        NativeScriptComponent() = default;
        explicit NativeScriptComponent(const std::string &dllPath)
        {
            dllLoader = DllLoader<ScriptableEntity>::create(dllPath);
        }

        template<typename T>
        void bind()
        {
            instantiateScript = []()
            {
                return Ref<ScriptableEntity>(new T(), [](T *pointer) { static_cast<ScriptableEntity *>(pointer)->onDestroy(); delete pointer; });
            };
        }

        void bindDLL()
        {
            instantiateScript = [this]() { return dllLoader->getClassInstance(); };
        }
    };

} // namespace Quasar
