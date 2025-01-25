#ifndef ENGINE_H_
#define ENGINE_H_

#include <tigr.h>

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
	void Update();
	void Render();
	void Quit();

	[[nodiscard]] bool get_is_running() const { return is_running_; }

private:
	bool is_running_{false};
	Tigr* window_{nullptr};
};

#endif // ENGINE_H_