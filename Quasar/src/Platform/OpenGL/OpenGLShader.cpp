#include "OpenGLShader.hpp"

#include "Quasar/Core/Base.hpp"
#include "Quasar/Core/Log.hpp"
#include "Quasar/Core/Timer.hpp"

#include "Quasar/System/FileSystem.hpp"

#include "Quasar/Debug/Instrumentor.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <fstream>
#include <vector>
#include <array>

namespace Quasar
{

    namespace Utils
    {

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

        static shaderc_shader_kind glShaderStageToShaderC(GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:
                    return shaderc_glsl_vertex_shader;
                case GL_FRAGMENT_SHADER:
                    return shaderc_glsl_fragment_shader;
                default:
                    QS_CORE_ASSERT(false);
                    return (shaderc_shader_kind)0;
            }
        }

        static const char *glShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:
                    return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER:
                    return "GL_FRAGMENT_SHADER";
                default:
                    QS_CORE_ASSERT(false);
                    return nullptr;
            }
        }

        static const char *glShaderStageCachedOpenGLFileExtension(uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:
                    return ".cached_opengl.vert";
                case GL_FRAGMENT_SHADER:
                    return ".cached_opengl.frag";
                default:
                    QS_CORE_ASSERT(false);
                    return "";
            }
        }

        static const char *glShaderStageCachedVulkanFileExtension(uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:
                    return ".cached_vulkan.vert";
                case GL_FRAGMENT_SHADER:
                    return ".cached_vulkan.frag";
                default:
                    QS_CORE_ASSERT(false);
                    return "";
            }
        }

    } // namespace Utils
    
    OpenGLShader::OpenGLShader(const std::string &filepath) 
        : m_Filepath(filepath)
    {
        QS_PROFILE_FUNCTION();

        FileSystem::createShaderCacheDirectoryIfNeeded(ShaderCacheAPI::OpenGL);
        FileSystem::createShaderCacheDirectoryIfNeeded(ShaderCacheAPI::Vulkan);

        std::string source = readFile(filepath);
        auto shaderSources = preProcess(source);

        {
            Timer timer;
            compileOrGetVulkanBinaries(shaderSources);
            compileOrGetOpenGLBinaries();
            createProgram();
            QS_CORE_WARN("Shader creation took {} ms", timer.ElapsedMillis());
        }

        std::filesystem::path path = filepath;
        m_Name = path.stem().string();
    }

    OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
        : m_Name(name)
    {
        QS_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        compileOrGetVulkanBinaries(sources);
        compileOrGetOpenGLBinaries();
        createProgram();
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
    
    void OpenGLShader::setFloat2(const std::string &name, const glm::vec2 &value) 
    {
        QS_PROFILE_FUNCTION();

        uploadUniformFloat2(name, value);
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

    std::string OpenGLShader::readFile(const std::string &filepath)
    {
        QS_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // Closes itself (RAII)
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
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
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
            QS_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file");
            size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after typeToken)
            std::string type = source.substr(begin, eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            QS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error in shader file");

            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
            shaderSources[Utils::shaderTypeFromString(type)] = pos == std::string::npos ?
                source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        QS_PROFILE_FUNCTION();

        GLuint program = glCreateProgram();

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = FileSystem::getShaderCacheDirectory(ShaderCacheAPI::Vulkan);

        auto &shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto &&[stage, source] : shaderSources)
        {
            std::filesystem::path shaderFilePath = m_Filepath;
            std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::glShaderStageCachedVulkanFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                shaderData[stage].resize(size / sizeof(uint32_t));
                in.read((char *)shaderData[stage].data(), size);
            }
            else
            {
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::glShaderStageToShaderC(stage), m_Filepath.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    QS_CORE_ERROR(module.GetErrorMessage());
                    QS_CORE_ASSERT(false);
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::binary);
                if (out.is_open())
                {
                    out.write((char *)shaderData[stage].data(), shaderData[stage].size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        for (auto &&[stage, data] : shaderData)
        {
            reflect(stage, data);
        }

    }

    void OpenGLShader::compileOrGetOpenGLBinaries()
    {
        QS_PROFILE_FUNCTION();
        // TODO: read from cache
        auto &shaderData = m_OpenGLSPIRV;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = FileSystem::getShaderCacheDirectory(ShaderCacheAPI::OpenGL);

        shaderData.clear();
        m_OpenGLSourceCode.clear();
        for (auto &&[stage, spirv] : m_VulkanSPIRV)
        {
            std::filesystem::path shaderFilePath = m_Filepath;
            std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::glShaderStageCachedOpenGLFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                shaderData[stage].resize(size / sizeof(uint32_t));
                in.read((char *)shaderData[stage].data(), size);
            }
            else
            {
                spirv_cross::CompilerGLSL glslCompiler(spirv);
                m_OpenGLSourceCode[stage] = glslCompiler.compile();
                auto &source = m_OpenGLSourceCode[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::glShaderStageToShaderC(stage), m_Filepath.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    QS_CORE_ERROR(module.GetErrorMessage());
                    QS_CORE_ASSERT(false);
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::binary);
                if (out.is_open())
                {
                    out.write((char *)shaderData[stage].data(), shaderData[stage].size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }

        }

    }

    void OpenGLShader::createProgram()
    {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto &&[stage, spirv] : m_OpenGLSPIRV)
        {
            GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), static_cast<int>(spirv.size() * sizeof(uint32_t)));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderID);
        }

        glLinkProgram(program);

        GLint isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            QS_CORE_ERROR("Shader Linking failed ({0}):\n{1}", m_Filepath, infoLog.data());

            glDeleteProgram(program);

            for (auto id : shaderIDs)
            {
                glDeleteShader(id);
            }
        }

        // TODO: should delete as well?
        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
        }

        m_RendererID = program;
    }

    void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t> &shaderData)
    {
        spirv_cross::Compiler compiler(shaderData);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        QS_CORE_TRACE("OpenGLShader::reflect - {0} {1}", Utils::glShaderStageToString(stage), m_Filepath);
        QS_CORE_TRACE("  {0} uniform buffers", resources.uniform_buffers.size());
        QS_CORE_TRACE("  {0} resources", resources.sampled_images.size());

        QS_CORE_TRACE("  Uniform buffers:");
        for (const auto &resource : resources.uniform_buffers)
        {
            const auto &bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            size_t memberCount = bufferType.member_types.size();

            QS_CORE_TRACE("    {0}", resource.name);
            QS_CORE_TRACE("      Size = {0}", bufferSize);
            QS_CORE_TRACE("      Binding = {0}", binding);
            QS_CORE_TRACE("      Members = {0}", memberCount);
        }
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

