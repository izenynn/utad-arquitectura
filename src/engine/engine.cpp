#include "engine.h"

#include "../logger/logger.h"
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/sprite_component.h"
#include "../systems/movement_system.h"
#include "../systems/render_system.h"

Engine::Engine()
{
	registry_ = std::make_unique<Registry>();
	asset_store_ = std::make_unique<AssetStore>();

	Logger::Info("Engine constructor called");
}

Engine::~Engine()
{
	Logger::Info("Engine destructor called");
}

void Engine::Init()
{
	window_ = tigrWindow(kWindowWidth, kWindowHeight, kWindowTitle, 0);
	if (window_ == nullptr) {
		Logger::Error("Failed to create a TIGR window");
		return;
	}

	is_running_ = true;
}

void Engine::Setup()
{
	// Add assets
	asset_store_->AddImage("player-image", "assets/player.png");

	// Add systems
	registry_->AddSystem<MovementSystem>();
	registry_->AddSystem<RenderSystem>();

	// Add entities
	Entity p = registry_->CreateEntity();
	p.AddComponent<TransformComponent>(glm::vec2(50.0f, 50.0f), glm::vec2(1.0f, 1.0f), 0.0f);
	p.AddComponent<RigidbodyComponent>(glm::vec2(20.0f, 1.0f));
	p.AddComponent<SpriteComponent>("player-image", 0);
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
	registry_->Update();
	registry_->GetSystem<MovementSystem>().Update(delta_time);
	registry_->GetSystem<RenderSystem>().Update(window_, asset_store_);

	tigrPrint(window_, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), sstr("DT: ", delta_time).c_str());
}

void Engine::Render()
{
	tigrUpdate(window_);
	tigrClear(window_, tigrRGB(0x80, 0x90, 0xa0));
}

void Engine::Quit()
{
	if (window_) {
		tigrFree(window_);
		window_ = nullptr;
	}
}

float Engine::Wait(float ms) const
{
	float time = tigrTime(); // Get time elapsed since last call
	ms += time * 1000; // Adjust ms to wait based on time elapsed since last call
	while (time * 1000 < ms)
		time += tigrTime();
	return time;
}