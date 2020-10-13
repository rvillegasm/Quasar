#include <Quasar.hpp>

class ExampleLayer : public Quasar::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void onUpdate() override
    {
        QS_INFO("ExampleLayer::Update");
    }

    void onEvent(Quasar::Event &event) override
    {
        QS_TRACE("{0}", event);
    }
};

class Sandbox : public Quasar::Application
{
public:
    Sandbox() 
    {
        pushLayer(new ExampleLayer());
        pushOverlay(new Quasar::ImGuiLayer());
    }

    ~Sandbox() 
    {
    }
};

Quasar::Application *Quasar::createApplication()
{
    return new Sandbox();
}