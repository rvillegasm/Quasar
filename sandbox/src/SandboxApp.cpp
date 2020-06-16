#include <Quasar.hpp>

class Sandbox : public Quasar::Application
{
public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

Quasar::Application *Quasar::createApplication()
{
    return new Sandbox();
}