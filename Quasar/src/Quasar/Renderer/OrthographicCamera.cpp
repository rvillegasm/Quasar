#include "OrthographicCamera.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{
    
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        QS_PROFILE_FUNCTION();

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
    
    void OrthographicCamera::setProjection(float left, float right, float bottom, float top) 
    {
        QS_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::recalculateViewMatrix()
    {
        QS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // because of glm, it has to be in this order
    }

    void OrthographicCamera::setPosition(const glm::vec3 &position)
    {
        m_Position = position; 
        recalculateViewMatrix();
    }

    void OrthographicCamera::setRotation(float rotation)
    {
        m_Rotation = rotation; 
        recalculateViewMatrix();
    }

} // namespace Quasar
