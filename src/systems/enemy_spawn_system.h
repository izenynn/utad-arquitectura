#ifndef ENEMY_SPAWN_SYSTEM_H_
#define ENEMY_SPAWN_SYSTEM_H_

#include <random>
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/tag_component.h"
#include "../components/rigidbody_component.h"
#include "../components/enemy_component.h"
#include "../components/sprite_component.h"

constexpr float kSpawnInterval = 5.0f; // Initial spawn interval in seconds

class EnemySpawnSystem : public System {
public:
	EnemySpawnSystem()
		: rng_(std::random_device{}()), dist_pos_x_(25.0f, 226.0f), dist_pos_y_(25.0f, 125.0f), dist_type_(0, 5), dist_dir_(-50.0f, 50.0f)
	{
		time_since_last_spawn_ = kSpawnInterval - 1.0f; // Start spawning immediately
		difficulty_timer_ = 0.0f;
		spawn_interval_ = kSpawnInterval; // Reset to initial spawn rate
	}

	void Update(float delta_time, Registry& registry)
	{
		if (!active_)
			return;

		time_since_last_spawn_ += delta_time;
		difficulty_timer_ += delta_time;

		if (time_since_last_spawn_ >= spawn_interval_) {
			SpawnEnemy(registry);
			time_since_last_spawn_ = 0.0f;
		}

		// Increase difficulty every 5 seconds
		if (difficulty_timer_ >= 5.0f && spawn_interval_ > 0.6f) {
			spawn_interval_ -= 0.2f;
			difficulty_timer_ = 0.0f;
		}
	}

	void Reset()
	{
		time_since_last_spawn_ = kSpawnInterval - 1.0f; // Start spawning immediately
		difficulty_timer_ = 0.0f;
		spawn_interval_ = kSpawnInterval; // Reset to initial spawn rate
		active_ = true;
	}

	void Stop()
	{
		active_ = false;
	}

private:
	float time_since_last_spawn_;
	float spawn_interval_;
	float difficulty_timer_;
	bool active_;

	std::mt19937 rng_;
	std::uniform_real_distribution<float> dist_pos_x_;
	std::uniform_real_distribution<float> dist_pos_y_;
	std::uniform_int_distribution<int> dist_type_;
	std::uniform_real_distribution<float> dist_dir_;

	void SpawnEnemy(Registry& registry)
	{
		glm::vec2 position(dist_pos_x_(rng_), 50.0f);
		glm::vec2 velocity;
		EnemyType type;
		int tier;

		int dist_type = dist_type_(rng_);
		if (dist_type == 0) {
			// Big bouncing ball
			type = EnemyType::BouncingBall;
			velocity = glm::vec2(dist_dir_(rng_), 0.0f);
			tier = 4;
		} else if (dist_type < 4) {
			// Bouncing ball
			type = EnemyType::BouncingBall;
			velocity = glm::vec2(dist_dir_(rng_), 0.0f);
			tier = 3;
		} else {
			// Hexagon ball
			type = EnemyType::HexagonBall;
			velocity = glm::vec2(dist_dir_(rng_), dist_dir_(rng_));
			tier = 3;

			position.y = dist_pos_y_(rng_);
		}

		auto e = registry.CreateEntity();
		e.AddComponent<TransformComponent>(position);
		e.AddComponent<TagComponent>("enemy");
		e.AddComponent<RigidbodyComponent>(velocity);
		e.AddComponent<EnemyComponent>(type, tier);

		std::string image = type == EnemyType::BouncingBall ? "ball" : "hex";
		e.AddComponent<SpriteComponent>(image + std::to_string(tier), tier);
	}
};

#endif // ENEMY_SPAWN_SYSTEM_H_
