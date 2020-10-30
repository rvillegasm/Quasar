#pragma once

#include "Quasar/Renderer/Shader.hpp"

#include <glm/glm.hpp>

namespace Quasar
{
    
    class OpenGLShader : public Shader
    {
    private:
        uint32_t m_RendererID;

    public:
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        virtual ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        void uploadUniformInt(const std::string& name, int value);

        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2 &values);
        void uploadUniformFloat3(const std::string& name, const glm::vec3 &values);
        void uploadUniformFloat4(const std::string& name, const glm::vec4 &values);

        void uploadUniformMat3(const std::string& name, const glm::mat3 &matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4 &matrix);
    };

} // namespace Quasar