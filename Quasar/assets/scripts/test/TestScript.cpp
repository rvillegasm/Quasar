#include "TestScript.h"

#include <iostream>

void hello()
{
    std::cout << "Hello, World!" << std::endl;
}

void Quasar::TestScript::onCreate()
{
    auto &translation = getComponent<TransformComponent>().translation;
    translation.x = rand() % 10 - 5.0f;
}

void Quasar::TestScript::onUpdate(Quasar::Timestep ts)
{
    auto &translation = getComponent<TransformComponent>().translation;

    float speed = 5.0f;

    // Right now, the quasar api can only be accessed through the ScriptableEntity parent class.
    // In order to access more functionality than the one already provided, extend said class.
    // E.g. add a isKeyPressed method and used it instead of the Input::isKeyPressed below.

//            if (Input::isKeyPressed(Key::J))
//            {
//                translation.x -= speed * ts;
//            }
//            if (Input::isKeyPressed(Key::L))
//            {
//                translation.x += speed * ts;
//            }
//            if (Input::isKeyPressed(Key::I))
//            {
//                translation.y += speed * ts;
//            }
//            if (Input::isKeyPressed(Key::K))
//            {
//                translation.y -= speed * ts;
//            }
}

#ifdef WIN32
#define SCRIPT_EXPORT __declspec(dllexport)
#else
#define SCRIPT_EXPORT
#endif

extern "C"
{
SCRIPT_EXPORT Quasar::TestScript *create()
{
    return new Quasar::TestScript();
}

SCRIPT_EXPORT void destroy(Quasar::TestScript *ptr)
{
    delete ptr;
}
}