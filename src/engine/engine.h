#ifndef ENGINE_H_
#define ENGINE_H_

#include <tigr.h>

constexpr unsigned int kFPS = 30;
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
};

#endif // ENGINE_H_