#pragma once

#ifdef MN_PLATFORM_WINDOWS

extern ModernEngine::Application* ModernEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	ModernEngine::Log::Init();

	MN_PROFILE_BEGIN_SESSION("Startup", "ModernEngineProfile-Startup.json");
	auto app = ModernEngine::CreateApplication({ argc, argv });
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("Runtime", "ModernEngineProfile-Runtime.json");
	app->Run();
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("Shutdown", "ModernEngineProfile-Shutdown.json");
	delete app;
	MN_PROFILE_END_SESSION();
}

#endif
