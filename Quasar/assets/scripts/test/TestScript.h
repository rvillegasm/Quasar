#pragma once

#include <Quasar.hpp>

void hello();

namespace Quasar
{
    class TestScript : public ScriptableEntity
    {
    public:
        void onCreate() override;

        void onDestroy() override
        {
        }

        void onUpdate(Quasar::Timestep ts) override;
    };
}