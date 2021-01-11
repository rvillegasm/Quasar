#pragma once

#include "Quasar/Core/Base.hpp"

#include "Quasar/Renderer/Texture.hpp"

#include <glm/glm.hpp>

namespace Quasar
{

    class SubTexture2D
    {
    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];

    public:
        SubTexture2D(const Ref<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max);

        const Ref<Texture2D> getTexture() const { return m_Texture; }
        const glm::vec2 *getTexCoords() const { return m_TexCoords; }

        static Ref<SubTexture2D> createFromCoords(
            const Ref<Texture2D> &texture,
            const glm::vec2 &coords,
            const glm::vec2 &subTextureSize,
            const glm::vec2 &subTextureAspectRatio = { 1, 1 }
        );
    };

} // namespace Quasar
