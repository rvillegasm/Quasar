#include "SceneHierarchyPanel.hpp"

#include <Quasar/Scene/Components.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace Quasar
{
    
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &context) 
    {
        setContext(context);
    }
    
    void SceneHierarchyPanel::setContext(const Ref<Scene> &context) 
    {
        m_Context = context;
    }
    
    void SceneHierarchyPanel::onImGuiRender() 
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{ entityID, m_Context.get() };
            drawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        {
            m_SelectionContext = {};
        }

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_SelectionContext)
        {
            drawComponents(m_SelectionContext);
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::drawEntityNode(Entity entity) 
    {
        auto &tag = entity.getComponent<TagComponent>().tag;
        
        ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }
    
    void SceneHierarchyPanel::drawComponents(Entity entity)
    {
        if (entity.hasComponent<TagComponent>())
        {
            auto &tag = entity.getComponent<TagComponent>().tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy(buffer, tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (entity.hasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void *)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto &transform = entity.getComponent<TransformComponent>().transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

                ImGui::TreePop();
            }
        }

        if (entity.hasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void *)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto &cameraComponent = entity.getComponent<CameraComponent>();
                SceneCamera &camera = cameraComponent.camera;

                ImGui::Checkbox("Primary", &cameraComponent.primary);

                const char *projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char *currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeStrings[i];
                            cameraComponent.camera.setProjectionType((SceneCamera::ProjectionType)i);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }

                if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float perspectiveVerticalFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
                    if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFOV))
                    {
                        camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFOV));
                    }
                    
                    float perspectiveNear = camera.getPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &perspectiveNear))
                    {
                        camera.setPerspectiveNearClip(perspectiveNear);
                    }

                    float perspectiveFar = camera.getPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &perspectiveFar))
                    {
                        camera.setPerspectiveFarClip(perspectiveFar);
                    }
                }

                if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float orthoSize = camera.getOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize))
                    {
                        camera.setOrthographicSize(orthoSize);
                    }
                    
                    float orthoNear = camera.getOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear))
                    {
                        camera.setOrthographicNearClip(orthoNear);
                    }

                    float orthoFar = camera.getOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar))
                    {
                        camera.setOrthographicFarClip(orthoFar);
                    }

                    ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
                }

                ImGui::TreePop();
            }
        }
    }

} // namespace Quasar
