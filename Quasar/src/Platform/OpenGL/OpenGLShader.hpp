#pragma once

#include "Quasar/Renderer/Shader.hpp"

#include <glad/glad.h>

namespace Quasar
{
    
    class OpenGLShader final : public Shader
    {
    private:
        uint32_t m_RendererID;
        std::string m_Name;

        mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

        std::string readFile(const std::string &filepath);
        std::unordered_map<GLenum, std::string> preProcess(const std::string &source);
        void compile(const std::unordered_map<GLenum, std::string> &shaderSources);
        
        GLint getUniformLocation(const std::string &name) const;

    public:
        OpenGLShader(const std::string &filepath);
        OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
        virtual ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        void setInt(const std::string &name, int value) override;
        void setIntArray(const std::string &name, int *values, uint32_t count) override;

        void setFloat(const std::string &name, float value) override;
        void setFloat2(const std::string &name, const glm::vec2 &value) override;
        void setFloat3(const std::string &name, const glm::vec3 &value) override;
        void setFloat4(const std::string &name, const glm::vec4 &value) override;

        void setMat4(const std::string &name, const glm::mat4 &value) override;

        const std::string &getName() const override { return m_Name; }

        void uploadUniformInt(const std::string& name, int value);
        void uploadUniformIntArray(const std::string& name, int *values, uint32_t count);

        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2 &values);
        void uploadUniformFloat3(const std::string& name, const glm::vec3 &values);
        void uploadUniformFloat4(const std::string& name, const glm::vec4 &values);

        void uploadUniformMat3(const std::string& name, const glm::mat3 &matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4 &matrix);
    };

} // namespace Quasar
