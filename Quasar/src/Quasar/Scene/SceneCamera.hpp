#pragma once

#include "Quasar/Renderer/Camera.hpp"

namespace Quasar
{
    
    class SceneCamera : public Camera
    {
    private:
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;

        void recalculateProjection();

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setOrthographic(float size, float nearClip, float farClip);

        void setViewportSize(uint32_t width, uint32_t height);

        float getOrthographicSize() const { return m_OrthographicSize; }
        void setOrthographicSize(float size) { m_OrthographicSize = size; recalculateProjection(); }
    };
    
} // namespace Quasar