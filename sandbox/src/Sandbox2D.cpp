#include "Sandbox2D.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
private:
    const char *m_Name;
    std::chrono::high_resolution_clock::time_point m_StartTimepoint;
    bool m_Stopped;
    Fn m_Func;

public:
    Timer(const char *name, Fn &&func)
        : m_Name(name), m_Stopped(false), m_Func(func)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
        {
            stop();
        }
    }

    void stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        m_Stopped = true;

        float duration = (end - start) * 0.001f;
        m_Func({ m_Name, duration });
    }
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach() 
{
    m_CheckerboardTexture = Quasar::Texture2D::create("/home/rvillegasm/dev/Quasar/sandbox/assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() 
{
    
}

void Sandbox2D::onUpdate(Quasar::Timestep ts) 
{
    PROFILE_SCOPE("Sandbox2D::onUpdate");

    // Update
    {
        PROFILE_SCOPE("CameraController::onUpdate");
        m_CameraController.onUpdate(ts);
    }

    // Render
    {
        PROFILE_SCOPE("Renderer Prep");
        Quasar::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Quasar::RenderCommand::clear();
    }

    {
        PROFILE_SCOPE("Renderer Draw");
        Quasar::Renderer2D::beginScene(m_CameraController.getCamera());
        Quasar::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Quasar::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
        Quasar::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() 
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    for (auto &result : m_ProfileResults)
    {
        char label[50];
        strcpy(label, "%.3fms ");
        strcat(label, result.name);
        ImGui::Text(label, result.time);
    }
    m_ProfileResults.clear();

    ImGui::End();
}

void Sandbox2D::onEvent(Quasar::Event &e) 
{
    m_CameraController.onEvent(e);
}
