#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Quasar
{
    
    class Shader
    {
    private:
        uint32_t m_RendererID;

    public:
        Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~Shader();

        void bind() const;
        void unbind() const;

        void uploadUniformMat4(const std::string& name, const glm::mat4 &matrix);
    };

} // namespace Quasar
