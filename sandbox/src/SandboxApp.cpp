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
        if (Quasar::Input::isKeyPressed(QS_KEY_TAB))
        {
            QS_TRACE("TAB key pressed! (polling)");
        }
    }

    void onEvent(Quasar::Event &event) override
    {
        if (event.getEventType() == Quasar::EventType::KeyPressed)
        {
            Quasar::KeyPressedEvent &e = (Quasar::KeyPressedEvent&)event;
            if (e.getKeyCode() == QS_KEY_TAB)
            {
                QS_TRACE("TAB key pressed! (event)");
            }
        }
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