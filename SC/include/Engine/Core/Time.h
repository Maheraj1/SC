#pragma once

#include "Core.h"
#include "Event.h"

namespace SC
{
    /**
     * @brief Class for time handling
     */
    class SC_API Time
    {
    private:
        static float fps_;
        static double time_;
        static double deltaTime_;
    public:

        // FPS Things
        static int targetFps;
        static bool CalculateFps;
        static bool LimitFPS;

        static float FixedUpdateRepetition;
        
        static const float& fps;
        static const double& time;
        static const double& deltaTime;

        static void Update();
        static void FixedUpdate();
    };
};
