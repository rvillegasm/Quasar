#pragma once

#include "Quasar/Renderer/Camera.hpp"

namespace Quasar
{
    
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };

    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;

        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f;
        float m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;

        void recalculateProjection();

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setPerspective(float verticalFov, float nearClip, float farClip);
        void setOrthographic(float size, float nearClip, float farClip);

        void setViewportSize(uint32_t width, uint32_t height);

        ProjectionType getProjectionType() const { return m_ProjectionType; }
        void setProjectionType(ProjectionType type) { m_ProjectionType = type; recalculateProjection(); }

        float getPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        void setPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; recalculateProjection(); }

        float getPerspectiveNearClip() const { return m_PerspectiveNear; }
        void setPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; recalculateProjection(); }

        float getPerspectiveFarClip() const { return m_PerspectiveFar; }
        void setPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; recalculateProjection(); }

        float getOrthographicSize() const { return m_OrthographicSize; }
        void setOrthographicSize(float size) { m_OrthographicSize = size; recalculateProjection(); }

        float getOrthographicNearClip() const { return m_OrthographicNear; }
        void setOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; recalculateProjection(); }

        float getOrthographicFarClip() const { return m_OrthographicFar; }
        void setOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; recalculateProjection(); }
    };
    
} // namespace Quasar