#include "engine.h"

#include "../logger/logger.h"

Engine::Engine()
{
	// TODO: make uniques

	Logger::Info("Engine constructor called");
}

Engine::~Engine()
{
	Logger::Info("Engine destructor called");
}

void Engine::Init()
{
	window_ = tigrWindow(320, 240, "Hello", 0);
	if (window_ == nullptr) {
		Logger::Error("Failed to create a TIGR window");
		return;
	}

	is_running_ = true;
}

void Engine::Setup()
{
	// TODO load game stuff
}

void Engine::Run()
{
	Setup();
	while (is_running_) {
		ProcessInput();
		Update();
		Render();
	}
}

void Engine::ProcessInput()
{
	is_running_ = !tigrClosed(window_) && !tigrKeyDown(window_, TK_ESCAPE);
}

void Engine::Update()
{
	// TODO: Logic

	// TODO: Collisions
}

void Engine::Render()
{
	tigrClear(window_, tigrRGB(0x80, 0x90, 0xa0));

	tigrPrint(window_, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), "Hello, world.");

	tigrUpdate(window_);
}

void Engine::Quit()
{
	tigrFree(window_);
	window_ = nullptr;
}