#pragma once

extern ModernEngine::Application* ModernEngine::CreateApplication();

int main(int argc, char** argv)
{
	ModernEngine::Log::Init();
	MN_CORE_INFO("Hello !");
	MN_TRACE("Client log is activated.");

	auto app = ModernEngine::CreateApplication();
	app->Run();
	delete app;
}
