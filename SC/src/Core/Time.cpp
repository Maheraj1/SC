#include "Engine/Core/Time.h"
#include "Engine/Core/Application.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Physics/Physics.h"
#include "b2_world.h"

#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

namespace SC
{
    int Time::fps_;
    double Time::time_;
    double Time::deltaTime_;
    double Time::FixedDeltaTime_;

    const double& Time::time = Time::time_;
    const double& Time::deltaTime = Time::deltaTime_;
    const double& Time::FixedDeltaTime = Time::FixedDeltaTime_;
    const int& Time::fps = Time::fps_;

    int Time::targetFps = 60;
    int Time::targetFixedUpdates = 5;
    bool Time::CalculateFps;

    static double lastFrame;

    void Time::Update()
    {
        using namespace std::literals::chrono_literals;
        time_ = glfwGetTime();
        deltaTime_ = time - lastFrame;

        if (CalculateFps) { fps_ = deltaTime * 1000.0f;}

        lastFrame = time;
        std::this_thread::sleep_for(1ms * ((1000.0f / (float)targetFps) - deltaTime));
        SceneManager::GetCurrentScene().Update();
        Debug::Log(std::to_string(fps));
    }

    void Time::FixedUpdate()
    {

        using namespace std::literals::chrono_literals;
        time_ = glfwGetTime();
        FixedDeltaTime_ = time - lastFrame;
        std::this_thread::sleep_for(1ms * ((1000.0f / (float)targetFixedUpdates) - FixedDeltaTime));
        Physics::GetWorld().Step(targetFixedUpdates / 10.0f, 6, 2);
        SceneManager::GetCurrentScene().FixedUpdate();
    }
}

