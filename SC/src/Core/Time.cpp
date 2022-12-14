#include "Engine/Core/Time.h"
#include "Engine/Core/Application.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Physics/Physics.h"
#include "b2_world.h"

#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

namespace SC
{
    float Time::fps_;
    double Time::time_;
    double Time::deltaTime_;

    const double& Time::time = Time::time_;
    const double& Time::deltaTime = Time::deltaTime_;
    const float& Time::fps = Time::fps_;

    int Time::targetFps = 100;
    bool Time::CalculateFps = true;
    bool Time::LimitFPS = true;

    float Time::FixedUpdateRepetition = 0.05f;

    static double lastFrame;
    static double accumulatorFixed;

    void Time::Update()
    {
        using namespace std::literals::chrono_literals;
        time_ = glfwGetTime() * 1000.0f;
        deltaTime_ = (time - lastFrame)/1000.0f;

        if (CalculateFps) { fps_ = (1000.0f / deltaTime)/1000.0f;}

        lastFrame = time;
        accumulatorFixed += deltaTime;
        if (LimitFPS) std::this_thread::sleep_for(1ms * ((1000.0f / targetFps) - deltaTime));
    }

    void Time::FixedUpdate()
    {
        if (accumulatorFixed >= FixedUpdateRepetition)
        {
            Physics::Step(FixedUpdateRepetition);
            Physics::UpdatePositions();
            accumulatorFixed = 0;
        }
    }
}

