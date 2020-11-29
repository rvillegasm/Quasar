#include <Quasar.hpp>
#include <Quasar/Core/EntryPoint.hpp>

#include "Sandbox2D.hpp"
#include "ExampleLayer.hpp"

class Sandbox : public Quasar::Application
{
public:
    Sandbox() 
    {
        // pushLayer(new ExampleLayer());
        pushLayer(new Sandbox2D());
    }

    ~Sandbox() 
    {
    }
};

Quasar::Application *Quasar::createApplication()
{
    return new Sandbox();
}