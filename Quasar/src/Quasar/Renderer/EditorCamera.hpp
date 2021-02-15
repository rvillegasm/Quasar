#pragma once

#include "Quasar/Renderer/Camera.hpp"
#include "Quasar/Core/Timestep.hpp"
#include "Quasar/Events/Event.hpp"
#include "Quasar/Events/MouseEvent.hpp"

#include <glm/glm.hpp>

namespace Quasar
{
    
    class EditorCamera : public Camera
    {
    private:
        float m_FOV = glm::radians(45.0f);
        float m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

        float m_Distance = 10.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;

        void updateProjection();
        void updateView();

        bool onMouseScroll(MouseScrolledEvent &e);

        void mousePan(const glm::vec2 &delta);
        void mouseRotate(const glm::vec2 &delta);
        void mouseZoom(float delta);

        glm::vec3 calculatePosition() const;
        
        glm::vec2 panSpeed() const;
        float rotationSpeed() const;
        float zoomSpeed() const;

    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void onUpdate(Timestep ts);
        void onEvent(Event &e);

        float getDistance() const { return m_Distance; }
        void setDistance(float distance) { m_Distance = distance; }

        void setViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; updateProjection(); }

        const glm::mat4 &getViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 getViewProjection() const { return m_Projection * m_ViewMatrix; }

        glm::vec3 getUpDirection() const;
        glm::vec3 getRightDirection() const;
        glm::vec3 getForwardDirection() const;
        const glm::vec3 &getPosition() const { return m_Position; }
        glm::quat getOrientation() const;

        float getPitch() const { return m_Pitch; }
        float getYaw() const { return m_Yaw; }
    };

} // namespace Quasar
