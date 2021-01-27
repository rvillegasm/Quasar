#pragma once

#include <glm/glm.hpp>

namespace Quasar
{
    
    class Camera
    {
    private:
        glm::mat4 m_Projection;

    public:
        Camera(const glm::mat4 &projection)
            : m_Projection(projection)
        {
        }

        const glm::mat4 &getProjection() const { return m_Projection; }
    };

} // namespace Quasar
