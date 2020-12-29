#pragma once

#include "Quasar/Core/Core.hpp"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace Quasar
{
    
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setInt(const std::string &name, int value) = 0;
        virtual void setIntArray(const std::string &name, int *values, uint32_t count) = 0;

        virtual void setFloat(const std::string &name, float value) = 0;

        virtual void setFloat3(const std::string &name, const glm::vec3 &value) = 0;
        virtual void setFloat4(const std::string &name, const glm::vec4 &value) = 0;
        
        virtual void setMat4(const std::string &name, const glm::mat4 &value) = 0;

        virtual const std::string &getName() const = 0;

        static Ref<Shader> create(const std::string &filepath);
        static Ref<Shader> create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
    };

    class ShaderLibrary
    {
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;

    public:
        void add(const Ref<Shader> &shader, const std::string &name);
        void add(const Ref<Shader> &shader);

        Ref<Shader> load(const std::string &filepath);
        Ref<Shader> load(const std::string &filepath, const std::string &name);

        Ref<Shader> get(const std::string &name);

        bool exists(const std::string &name) const;
    };

} // namespace Quasar
