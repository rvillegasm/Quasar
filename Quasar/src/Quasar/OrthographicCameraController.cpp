#include "Quasar/OrthographicCameraController.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Input.hpp"
#include "Quasar/KeyCodes.hpp"

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
        if (Input::isKeyPressed(QS_KEY_A))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(QS_KEY_D))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        }

        if (Input::isKeyPressed(QS_KEY_W))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        }
        else if (Input::isKeyPressed(QS_KEY_S))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        }

        if (m_WithRotation)
        {
            if (Input::isKeyPressed(QS_KEY_Q))
            {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            else if (Input::isKeyPressed(QS_KEY_E))
            {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }

            m_Camera.setRotation(m_CameraRotation);
            
            m_CameraTranslationSpeed = m_ZoomLevel;
        }

        m_Camera.setPosition(m_CameraPosition);
    }
    
    void OrthographicCameraController::onEvent(Event &e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(QS_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(QS_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &e) 
    {
        m_ZoomLevel -= e.getYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }
    
    bool OrthographicCameraController::onWindowResized(WindowResizeEvent &e) 
    {
        m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
        m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }

} // namespace Quasar
