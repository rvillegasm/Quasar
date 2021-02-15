#include "EditorCamera.hpp"

#include "Quasar/Core/Input.hpp"
#include "Quasar/Core/KeyCodes.hpp"
#include "Quasar/Core/MouseCodes.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Quasar
{

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : Camera(glm::perspective(fov, aspectRatio, nearClip, farClip)),
          m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
          
    {
        updateView();
    }
    
    void EditorCamera::onUpdate(Timestep ts) 
    {
        if (Input::isKeyPressed(Key::LeftAlt))
        {
            const glm::vec2 &mouse = Input::getMousePosition();
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = mouse;

            if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
            {
                mousePan(delta);
            }
            else if (Input::isMouseButtonPressed(Mouse::ButtonLeft))
            {
                mouseRotate(delta);
            }
            else if (Input::isMouseButtonPressed(Mouse::ButtonRight))
            {
                mouseZoom(delta.y);
            }
        }

        updateView();
    }
    
    void EditorCamera::onEvent(Event &e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(QS_BIND_EVENT_FN(EditorCamera::onMouseScroll));
    }

    glm::vec3 EditorCamera::getUpDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    glm::vec3 EditorCamera::getRightDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    
    glm::vec3 EditorCamera::getForwardDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    
    glm::quat EditorCamera::getOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

    void EditorCamera::updateProjection() 
    {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::updateView()
    {
        // m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
        m_Position = calculatePosition();

        glm::quat orientation = getOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }
    
    bool EditorCamera::onMouseScroll(MouseScrolledEvent &e) 
    {
        float delta = e.getYOffset() * 0.1f;
        mouseZoom(delta);
        updateView();
        return false;
    }
    
    void EditorCamera::mousePan(const glm::vec2 &delta) 
    {
        const glm::vec2 &speed = panSpeed();
        m_FocalPoint += -getRightDirection() * delta.x * speed.x * m_Distance;
        m_FocalPoint += getUpDirection() * delta.y * speed.y * m_Distance;
    }
    
    void EditorCamera::mouseRotate(const glm::vec2 &delta) 
    {
        float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * rotationSpeed();
        m_Pitch += delta.y * rotationSpeed();
    }
    
    void EditorCamera::mouseZoom(float delta) 
    {
        m_Distance -= delta * zoomSpeed();
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += getForwardDirection();
            m_Distance = 1.0f;
        }
    }
    
    glm::vec3 EditorCamera::calculatePosition() const
    {
        return m_FocalPoint - getForwardDirection() * m_Distance;
    }
    
    glm::vec2 EditorCamera::panSpeed() const
    {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor }; 
    }
    
    float EditorCamera::rotationSpeed() const
    {
        return 0.8f;
    }
    
    float EditorCamera::zoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
    }

} // namespace Quasar
