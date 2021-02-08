#pragma once

#include "Quasar/Core/Base.hpp"
#include "Quasar/Scene/Scene.hpp"

namespace Quasar
{
    
    class SceneSerializer
    {
    private:
        Ref<Scene> m_Scene;

    public:
        SceneSerializer(const Ref<Scene> &scene);

        void serializeToText(const std::string &filepath);
        void serializeToBinary(const std::string &filepath);

        bool deserializeFromText(const std::string &filepath);
        bool deserializeFromBinary(const std::string &filepath);
    };

} // namespace Quasar
