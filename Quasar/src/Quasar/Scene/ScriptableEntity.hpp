#pragma once

#include "Quasar/Core/Timestep.hpp"
#include "Quasar/Scene/Entity.hpp"

namespace Quasar
{
    
    class ScriptableEntity
    {
        friend class Scene;

    private:
        Entity m_Entity;

    protected:
        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(Timestep ts) {}

    public:
        virtual ~ScriptableEntity() = default;

        template<typename T>
        T &getComponent()
        {
            return m_Entity.getComponent<T>();
        }
    };

} // namespace Quasar
