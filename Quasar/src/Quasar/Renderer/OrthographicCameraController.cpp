#include "OrthographicCameraController.hpp"

#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/Input.hpp"
#include "Quasar/Core/KeyCodes.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

namespace Quasar
{
    
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool withRotation)
        : m_AspectRatio(aspectRatio),
          m_WithRotation(withRotation),
          m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    {
    }
    
    void OrthographicCameraController::onUpdate(Timestep ts) 
    {
        QS_PROFILE_FUNCTION();

        if (Input::isKeyPressed(Key::A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (Input::isKeyPressed(Key::W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        if (m_WithRotation)
        {
            if (Input::isKeyPressed(Key::Q))
            {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            else if (Input::isKeyPressed(Key::E))
            {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }

            if (m_CameraRotation > 180.0f)
            {
                m_CameraRotation -= 360.0f;
            }
            else if (m_CameraRotation <= -180.0f)
            {
                m_CameraRotation += 360.0f;
            }

            m_Camera.setRotation(m_CameraRotation);
        }

        m_Camera.setPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }
    
    void OrthographicCameraController::onEvent(Event &e) 
    {
        QS_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(QS_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(QS_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }
    
    void OrthographicCameraController::onResize(float width, float height) 
    {
        QS_PROFILE_FUNCTION();

        m_AspectRatio = width / height;
        m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &e) 
    {
        QS_PROFILE_FUNCTION();

        m_ZoomLevel -= e.getYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }
    
    bool OrthographicCameraController::onWindowResized(WindowResizeEvent &e) 
    {
        QS_PROFILE_FUNCTION();

        onResize((float)e.getWidth(), (float)e.getHeight());
        return false;
    }

} // namespace Quasar
