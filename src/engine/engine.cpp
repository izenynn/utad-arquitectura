#include "engine.h"

#include <windows.h>

#include "../components/input_component.h"
#include "../logger/logger.h"
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/sprite_component.h"
#include "../components/enemy_component.h"
#include "../systems/movement_system.h"
#include "../systems/render_system.h"
#include "../systems/input_system.h"
#include "../systems/player_control_system.h"
#include "../systems/enemy_control_system.h"
#include "../systems/enemy_spawn_system.h"

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
	asset_store_->AddImage("player", "assets/player.png");
	asset_store_->AddImage("background", "assets/background.png");
	asset_store_->AddImage("ball1", "assets/ball1.png");
	asset_store_->AddImage("ball2", "assets/ball2.png");
	asset_store_->AddImage("ball3", "assets/ball3.png");
	asset_store_->AddImage("ball4", "assets/ball4.png");
	asset_store_->AddImage("hex1", "assets/hex1.png");
	asset_store_->AddImage("hex2", "assets/hex2.png");
	asset_store_->AddImage("hex3", "assets/hex3.png");

	// Add systems
	registry_->AddSystem<MovementSystem>();
	registry_->AddSystem<RenderSystem>();
	registry_->AddSystem<InputSystem>();
	registry_->AddSystem<PlayerControlSystem>();
	registry_->AddSystem<EnemyControlSystem>();
	registry_->AddSystem<EnemySpawnSystem>();

	// Add entities
	Entity p = registry_->CreateEntity();
	p.AddComponent<TransformComponent>(glm::vec2(128.0f, 176.0f));
	p.AddComponent<RigidbodyComponent>();
	p.AddComponent<InputComponent>();
	p.AddComponent<SpriteComponent>("player", 10);

	Entity bg = registry_->CreateEntity();
	bg.AddComponent<TransformComponent>();
	bg.GetComponent<TransformComponent>().position = glm::vec2(0.0f, 8.0f);
	bg.GetComponent<TransformComponent>().pivot = glm::vec2(0.0f, 0.0f);
	bg.AddComponent<SpriteComponent>("background", 0);

	Entity enemy = registry_->CreateEntity();
	enemy.AddComponent<TransformComponent>(glm::vec2(100.0f, 100.0f));
	enemy.AddComponent<RigidbodyComponent>(glm::vec2(25.0f, 0.0f));
	enemy.AddComponent<SpriteComponent>("ball1", 20);
	enemy.AddComponent<EnemyComponent>();
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
	registry_->GetSystem<InputSystem>().Update(window_);
	registry_->GetSystem<PlayerControlSystem>().Update(delta_time);
	registry_->GetSystem<EnemyControlSystem>().Update(delta_time, *registry_, *asset_store_, kWindowWidth, kWindowHeight);
	registry_->GetSystem<MovementSystem>().Update(delta_time);
	registry_->GetSystem<RenderSystem>().Update(window_, asset_store_);

	// tigrPrint(window_, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), sstr("DT: ", delta_time).c_str());
}

void Engine::Render()
{
	tigrUpdate(window_);
	tigrClear(window_, tigrRGB(0x0c, 0x0c, 0x0c));
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