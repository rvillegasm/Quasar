#include "OpenGLShader.hpp"

#include "Quasar/Core/Core.hpp"
#include "Quasar/Core/Log.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <filesystem>
#include <vector>
#include <array>

namespace Quasar
{
    
    OpenGLShader::OpenGLShader(const std::string &filepath) 
    {
        QS_PROFILE_FUNCTION();

        std::string source = readFile(filepath);
        auto shaderSources = preProcess(source);
        compile(shaderSources);

        std::filesystem::path path = filepath;
        m_Name = path.stem().string();
    }

    OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
        : m_Name(name)
    {
        QS_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources.reserve(2);
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        QS_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::bind() const
    {
        QS_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }

    void OpenGLShader::unbind() const
    {
        QS_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::setInt(const std::string &name, int value)
    {
        QS_PROFILE_FUNCTION();

        uploadUniformInt(name, value);
    }

    void OpenGLShader::setIntArray(const std::string &name, int *values, uint32_t count)
    {
        QS_PROFILE_FUNCTION();

        uploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::setFloat(const std::string &name, float value)
    {
        QS_PROFILE_FUNCTION();

        uploadUniformFloat(name, value);
    }

    void OpenGLShader::setFloat3(const std::string &name, const glm::vec3 &value) 
    {
        QS_PROFILE_FUNCTION();
        
        uploadUniformFloat3(name, value);
    }
    
    void OpenGLShader::setFloat4(const std::string &name, const glm::vec4 &value) 
    {
        QS_PROFILE_FUNCTION();

        uploadUniformFloat4(name, value);
    }
    
    void OpenGLShader::setMat4(const std::string &name, const glm::mat4 &value) 
    {
        QS_PROFILE_FUNCTION();

        uploadUniformMat4(name, value);
    }

    void OpenGLShader::uploadUniformInt(const std::string& name, int value)
    {
        // make sure to bind before calling
        GLint location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformIntArray(const std::string& name, int *values, uint32_t count)
    {
        GLint location = getUniformLocation(name);
        glUniform1iv(location, count, values);
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
        QS_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
                in.close();
            }
            else
            {
                QS_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            QS_CORE_ERROR("Could not open shader file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string &source)
    {
        QS_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        size_t typeTokenLenght = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
            QS_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file");
            size_t begin = pos + typeTokenLenght + 1; // Start of shader type name (after typeToken)
            std::string type = source.substr(begin, eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            QS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error in shader file");

            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
            shaderSources[shaderTypeFromString(type)] = pos == std::string::npos ? 
                source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        QS_PROFILE_FUNCTION();

        // taken from: https://www.khronos.org/opengl/wiki/Shader_Compilation#Example

        GLuint program = glCreateProgram();
        QS_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders are supported for now!");
        std::array<GLuint, 2> glShaderIDs;

        int glShaderIDsIndex = 0;
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
            glShaderIDs[glShaderIDsIndex++] = shader;
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
            glDeleteShader(id);
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

