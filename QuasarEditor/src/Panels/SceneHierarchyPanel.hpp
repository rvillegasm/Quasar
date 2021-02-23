#pragma once

#include <Quasar/Core/Base.hpp>
#include <Quasar/Scene/Scene.hpp>
#include <Quasar/Scene/Entity.hpp>

namespace Quasar
{
    
    class SceneHierarchyPanel
    {
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;

        void drawEntityNode(Entity entity);
        void drawComponents(Entity entity);

    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene> &scene);

        void setContext(const Ref<Scene> &scene);

        void onImGuiRender();

        Entity getSelectedEntity() const { return m_SelectionContext; }
        void setSelectedEntity(Entity entity);
    };

} // namespace Quasar
