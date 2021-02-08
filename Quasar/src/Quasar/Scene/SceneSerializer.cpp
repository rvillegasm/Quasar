#include "SceneSerializer.hpp"

#include "Quasar/Scene/Entity.hpp"
#include "Quasar/Scene/Components.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML
{

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3 &rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node &node, glm::vec3 &rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4 &rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node &node, glm::vec4 &rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
    
} // namespace YAML

namespace Quasar
{

    YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene> &scene) 
        : m_Scene(scene)
    {
    }
    
    static void serializeEntity(YAML::Emitter &out, Entity entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: entity id goes here

        if (entity.hasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // Tag
            auto &tag = entity.getComponent<TagComponent>().tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;
            out << YAML::EndMap; // Tag
        }

        if (entity.hasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // transform
            auto &transformComponent = entity.getComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << transformComponent.translation;
            out << YAML::Key << "Rotation" << YAML::Value << transformComponent.rotation;
            out << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;
            out << YAML::EndMap; // transform
        }

        if (entity.hasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // camera component

            auto &cameraComponent = entity.getComponent<CameraComponent>();
            auto &camera = cameraComponent.camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; // camera
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
            out << YAML::EndMap; // camera

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
            out << YAML::EndMap; // camera component
        }

        if (entity.hasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // sprite renderer
            auto &spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;
            out << YAML::EndMap; // sprite renderer
        }

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::serializeToText(const std::string &filepath) 
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << "Untitled Scene";
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.each([&](auto entityID)
        {
            Entity entity = { entityID, m_Scene.get() };
            if (!entity)
            {
                return;
            }

            serializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }
    
    void SceneSerializer::serializeToBinary(const std::string &filepath) 
    {
        // Not yet implemented
        QS_CORE_ASSERT(false);
    }
    
    bool SceneSerializer::deserializeFromText(const std::string &filepath) 
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
        {
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();
        QS_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        YAML::Node entitiesNode = data["Entities"];
        if (entitiesNode)
        {
            for (auto entityNode : entitiesNode)
            {
                uint64_t uuid = entityNode["Entity"].as<uint64_t>(); // TODO: uuid
                
                std::string name;
                auto tagComponentNode = entityNode["TagComponent"];
                if (tagComponentNode)
                {
                    name = tagComponentNode["Tag"].as<std::string>();
                }
                QS_CORE_TRACE("Deserialized entity with ID = {0}, name = '{1}'", uuid, name);

                Entity deserializedEntity = m_Scene->createEntity(name);

                auto transformComponentNode = entityNode["TransformComponent"];
                if (transformComponentNode)
                {
                    // Entities always have transforms by default
                    auto &transformComponent = deserializedEntity.getComponent<TransformComponent>();
                    transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
                    transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
                    transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
                }

                auto cameraComponentNode = entityNode["CameraComponent"];
                if (cameraComponentNode)
                {
                    auto &cameraComponent = deserializedEntity.addComponent<CameraComponent>();

                    auto cameraPropsNode = cameraComponentNode["Camera"];
                    cameraComponent.camera.setProjectionType((SceneCamera::ProjectionType)cameraPropsNode["ProjectionType"].as<int>());

                    cameraComponent.camera.setPerspectiveVerticalFOV(cameraPropsNode["PerspectiveFOV"].as<float>());
                    cameraComponent.camera.setPerspectiveNearClip(cameraPropsNode["PerspectiveNear"].as<float>());
                    cameraComponent.camera.setPerspectiveFarClip(cameraPropsNode["PerspectiveFar"].as<float>());

                    cameraComponent.camera.setOrthographicSize(cameraPropsNode["OrthographicSize"].as<float>());
                    cameraComponent.camera.setOrthographicNearClip(cameraPropsNode["OrthographicNear"].as<float>());
                    cameraComponent.camera.setOrthographicFarClip(cameraPropsNode["OrthographicFar"].as<float>());

                    cameraComponent.primary = cameraComponentNode["Primary"].as<bool>();
                    cameraComponent.fixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();
                }

                auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"];
                if (spriteRendererComponentNode)
                {
                    auto &spriteRendererComponent = deserializedEntity.addComponent<SpriteRendererComponent>();
                    spriteRendererComponent.color = spriteRendererComponentNode["Color"].as<glm::vec4>();
                }
            }
        }

        return true;
    }
    
    bool SceneSerializer::deserializeFromBinary(const std::string &filepath) 
    {
        // Not yet implemented
        QS_CORE_ASSERT(false);
        return false;
    }

} // namespace Quasar
