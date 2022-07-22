#include "Engine/Core/Time.h"
#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

namespace SC
{
    int Time::fps_;
    double Time::time_;
    double Time::deltaTime_;

    const double& Time::time = Time::time_;
    const double& Time::deltaTime = Time::deltaTime_;
    const int& Time::fps = Time::fps_;

    int Time::targetFps = 60;
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
    }
}

