#ifndef ENGINE_H_
#define ENGINE_H_

#include <memory>

#include <tigr.h>

#include "../ecs/ecs.h"
#include "../asset_store/asset_store.h"

constexpr int kWindowWidth = 320;
constexpr int kWindowHeight = 240;
constexpr const char* kWindowTitle = "Daniel Poveda";

constexpr unsigned int kFPS = 60;
constexpr unsigned int kMsPerFrame = 1000 / kFPS;

class Engine {
public:
	Engine();
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

	void Init();
	void Run();
	void Setup();
	void ProcessInput();
	void Update(float delta_time);
	void Render();
	void Quit();

	[[nodiscard]] bool get_is_running() const { return is_running_; }

private:
	float Wait(float ms) const;

	bool is_running_{false};
	Tigr* window_{nullptr};

	std::unique_ptr<Registry> registry_{nullptr};
	std::unique_ptr<AssetStore> asset_store_{nullptr};
};

#endif // ENGINE_H_