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
        static int fps_;
        static double time_;
        static double deltaTime_;
        static double FixedDeltaTime_;
    public:

        // FPS Things
        static int targetFps;
        static int targetFixedUpdates;
        static bool CalculateFps;
        
        static const int& fps;
        static const double& time;
        static const double& deltaTime;
        static const double& FixedDeltaTime;

        static void Update();
        static void FixedUpdate();
    };
};
