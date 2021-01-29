#include "SceneHierarchyPanel.hpp"

#include <Quasar/Scene/Components.hpp>

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

} // namespace Quasar
