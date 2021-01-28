#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Quasar
{

    SceneCamera::SceneCamera() 
    {
        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip) 
    {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;

        recalculateProjection();
    }
    
    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) 
    {
        m_AspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() 
    {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);        
    }

} // namespace Quasar
