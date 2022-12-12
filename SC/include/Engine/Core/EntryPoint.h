#pragma once

#include "Application.h"
#include "Engine/Resources/Resources.h"

extern void PreAppRun();

inline void RunApp()
{
	SC::Debug::Info("Starting Engine", "SC::Application");
	SC::Application app;
	app.Run(PreAppRun);
}

int main()
{
	RunApp();
}