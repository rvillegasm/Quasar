#include "OpenGLShader.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

namespace Quasar
{

    OpenGLShader::OpenGLShader(const std::string &filepath) 
    {
        std::string source = readFile(filepath);
        auto shaderSources = preProcess(source);
        compile(shaderSources); 
    }

    OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        std::unordered_map<GLenum, std::string> sources(2);
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::uploadUniformInt(const std::string& name, int value)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2 &values)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3 &values)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4 &values)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3 &matrix)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4 &matrix)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    // ----- PRIVATE -----

    static GLenum shaderTypeFromString(const std::string &type)
    {
        if (type == "vertex")
        {
            return GL_VERTEX_SHADER;
        }
        else if (type == "fragment" || type == "pixel")
        {
            return GL_FRAGMENT_SHADER;
        }
        else
        {
            QS_CORE_ASSERT(false, "Unkown shader type!");
            return 0;
        }
    }

    std::string OpenGLShader::readFile(const std::string &filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            QS_CORE_ERROR("Could not open shader file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string &source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        size_t typeTokenLenght = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            QS_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file");
            size_t begin = pos + typeTokenLenght + 1;
            std::string type = source.substr(begin, eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[shaderTypeFromString(type)] = source.substr(
                nextLinePos,
                pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
            );
        }

        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        // taken from: https://www.khronos.org/opengl/wiki/Shader_Compilation#Example

        GLuint program = glCreateProgram();
        std::vector<GLuint> glShaderIDs(shaderSources.size());

        for (auto &[type, source] : shaderSources)
        {
            GLuint shader = glCreateShader(type);

            const GLchar *sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                QS_CORE_ERROR("{0}", infoLog.data());
                QS_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs.push_back(shader);
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            for (auto id : glShaderIDs)
            {
                glDeleteShader(id);
            }

            QS_CORE_ERROR("{0}", infoLog.data());
            QS_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
        }

        m_RendererID = program;
    }

    GLint OpenGLShader::getUniformLocation(const std::string &name) const
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        {
            return m_UniformLocationCache[name];
        }
        else
        {
            GLint location = glGetUniformLocation(m_RendererID, name.c_str());
            m_UniformLocationCache[name] = location;
            return location;
        }
    }

} // namespace Quasar

