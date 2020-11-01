#pragma once

#include "Quasar/Core/Timestep.hpp"

#include "Quasar/Renderer/OrthographicCamera.hpp"

#include "Quasar/Events/Event.hpp"
#include "Quasar/Events/ApplicationEvent.hpp"
#include "Quasar/Events/MouseEvent.hpp"

namespace Quasar
{
    
    class OrthographicCameraController
    {
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_WithRotation;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;

        float m_CameraTranslationSpeed = 5.0f;
        float m_CameraRotationSpeed = 180.0f;

        bool onMouseScrolled(MouseScrolledEvent &e);
        bool onWindowResized(WindowResizeEvent &e);

    public:
        OrthographicCameraController(float aspectRatio, bool withRotation = false);

        void onUpdate(Timestep ts);
        void onEvent(Event &e);

        OrthographicCamera &getCamera() { return m_Camera; }
        const OrthographicCamera &getCamera() const { return m_Camera; }
    };

} // namespace Quasar

