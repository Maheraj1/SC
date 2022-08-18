#pragma once

#include "Application.h"

extern void PreAppRun();

inline void RunApp()
{
	SC::Debug::Info("Starting Engine", "SC::Application");
	SC::Application app;
	app.Run(PreAppRun);
}
