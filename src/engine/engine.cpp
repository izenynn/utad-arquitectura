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
	float elapsed_time = tigrTime();
	float prev_frame_time = 0.0f;

	Setup();
	while (is_running_) {
		elapsed_time += tigrTime(); // Update elapsed time

		float wait_time = kMsPerFrame - ((elapsed_time - prev_frame_time) * 1000);
		if (wait_time > 0.0f && wait_time <= kMsPerFrame)
			elapsed_time += Wait(wait_time);

		float delta_time = elapsed_time - prev_frame_time;
		prev_frame_time = elapsed_time;

		ProcessInput();
		Update(delta_time);
		Render();
	}
}

void Engine::ProcessInput()
{
	is_running_ = !tigrClosed(window_) && !tigrKeyDown(window_, TK_ESCAPE);
}

void Engine::Update(float delta_time)
{
	(void)delta_time;

	// TODO: Logic

	// TODO: Collisions

	// TODO: temporal for debuging
	tigrClear(window_, tigrRGB(0x80, 0x90, 0xa0));
	tigrPrint(window_, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), sstr("DT: ", delta_time).c_str());
}

void Engine::Render()
{
	//tigrClear(window_, tigrRGB(0x80, 0x90, 0xa0));

	//tigrPrint(window_, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), "Hello, world.");

	tigrUpdate(window_);
}

void Engine::Quit()
{
	tigrFree(window_);
	window_ = nullptr;
}

float Engine::Wait(float ms) const
{
	float time = tigrTime(); // Get time elapsed since last call
	ms += time * 1000; // Adjust ms to wait based on time elapsed since last call
	while (time * 1000 < ms)
		time += tigrTime();
	return time;
}