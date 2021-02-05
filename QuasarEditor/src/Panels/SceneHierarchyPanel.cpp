#include "SceneHierarchyPanel.hpp"

#include <Quasar/Scene/Components.hpp>

#include <imgui.h>
#include <imgui_internal.h>

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

        // Right click on a blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                m_Context->createEntity("Empty Entity");
            }

            ImGui::EndPopup();
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
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                // Defer the destruction to after all the ImGui code is run, to avoid dangling references.
                entityDeleted = true;
            }

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Context->destroyEntity(entity);
            if (m_SelectionContext == entity)
            {
                m_SelectionContext = {};
            }
        }
    }
    
    static void drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO &io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template<typename T, typename UIFunc>
    static void drawComponent(const std::string &name, Entity entity, UIFunc uiFunc)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.hasComponent<T>())
        {
            auto &component = entity.getComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                {
                    removeComponent = true;
                }
            
                ImGui::EndPopup();
            }

            if (open)
            {

                uiFunc(component);
                ImGui::TreePop();
            }

            if (removeComponent)
            {
                entity.removeComponent<T>();
            }
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
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                m_SelectionContext.addComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer"))
            {
                m_SelectionContext.addComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        
        ImGui::PopItemWidth();

        drawComponent<TransformComponent>("Transform", entity, [](auto &component)
        {
            drawVec3Control("Translation", component.translation);
            glm::vec3 rotation = glm::degrees(component.rotation);
            drawVec3Control("Rotation", rotation);
            component.rotation = glm::radians(rotation);
            drawVec3Control("Scale", component.scale, 1.0f);
        });

        drawComponent<CameraComponent>("Camera", entity, [](auto &component)
        {
            SceneCamera &camera = component.camera;

            ImGui::Checkbox("Primary", &component.primary);

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
                        component.camera.setProjectionType((SceneCamera::ProjectionType)i);
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

                ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
            }
        });

        drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto &component)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
        });
    }

} // namespace Quasar
