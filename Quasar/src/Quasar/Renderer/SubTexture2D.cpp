#include "SubTexture2D.hpp"

namespace Quasar
{

    SubTexture2D::SubTexture2D(const Ref<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max) 
        : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }
    
    Ref<SubTexture2D> SubTexture2D::createFromCoords(
        const Ref<Texture2D> &texture,
        const glm::vec2 &coords,
        const glm::vec2 &subTextureSize,
        const glm::vec2 &subTextureAspectRatio) 
    {
        glm::vec2 min = { 
            (coords.x * subTextureSize.x) / texture->getWidth(),
            (coords.y * subTextureSize.y) / texture->getHeight()
        };
        glm::vec2 max = {
            ((coords.x + subTextureAspectRatio.x) * subTextureSize.x) / texture->getWidth(),
            ((coords.y + subTextureAspectRatio.y) * subTextureSize.y) / texture->getHeight()
        };

        return createRef<SubTexture2D>(texture, min, max);
    }
    
} // namespace Quasar
