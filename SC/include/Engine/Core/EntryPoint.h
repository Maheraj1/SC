#pragma once

#include "Application.h"
#include "Engine/Resources/Resources.h"

extern void PreAppRun();
extern void AddApplicationAddons();

inline void RunApp()
{
	SC::Debug::Info("Starting Engine", "SC::Application");
	SC::Application app;
	app.Run(PreAppRun, AddApplicationAddons);
}

int main()
{
	RunApp();
}