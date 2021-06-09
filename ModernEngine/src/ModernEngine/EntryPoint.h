#pragma once

extern ModernEngine::Application* ModernEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = ModernEngine::CreateApplication();
	app->Run();
	delete app;
}
