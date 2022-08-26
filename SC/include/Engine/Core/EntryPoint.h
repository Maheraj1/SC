#pragma once

#include "Application.h"

namespace SC::Internal::Components {extern void ComponentsFunc();}

extern void PreAppRun();


inline void RunApp()
{
	SC::Debug::Info("Starting Engine", "SC::Application");
	SC::Application app;
	app.Run(PreAppRun);
}
