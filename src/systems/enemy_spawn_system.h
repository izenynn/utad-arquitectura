#ifndef ENEMY_SPAWN_SYSTEM_H_
#define ENEMY_SPAWN_SYSTEM_H_

#include <random>
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/enemy_component.h"
#include "../components/sprite_component.h"

class EnemySpawnSystem : public System {
public:
	EnemySpawnSystem()
		: rng_(std::random_device{}()), dist_pos_x_(20.0f, 300.0f), dist_type_(0, 1), dist_dir_(-60.0f, 60.0f)
	{
		time_since_last_spawn_ = 0.0f;
		spawn_interval_ = 2.0f; // Initial spawn every 2 seconds
		difficulty_timer_ = 0.0f;
	}

	void Update(float delta_time, Registry& registry)
	{
		(void)delta_time;
		(void)registry;
		/*time_since_last_spawn_ += delta_time;
		difficulty_timer_ += delta_time;

		if (time_since_last_spawn_ >= spawn_interval_) {
			SpawnEnemy(registry);
			time_since_last_spawn_ = 0.0f;
		}

		// Increase difficulty every 20 seconds
		if (difficulty_timer_ >= 20.0f && spawn_interval_ > 0.6f) {
			spawn_interval_ -= 0.2f;
			difficulty_timer_ = 0.0f;
		}*/
	}

private:
	float time_since_last_spawn_;
	float spawn_interval_;
	float difficulty_timer_;

	std::mt19937 rng_;
	std::uniform_real_distribution<float> dist_pos_x_;
	std::uniform_int_distribution<int> dist_type_;
	std::uniform_real_distribution<float> dist_dir_;

	void SpawnEnemy(Registry& registry)
	{
		glm::vec2 position(dist_pos_x_(rng_), 0.0f);
		glm::vec2 velocity;
		EnemyType type;
		int tier;

		if (dist_type_(rng_) == 0) {
			// Bouncing ball
			type = EnemyType::BouncingBall;
			velocity = glm::vec2(dist_dir_(rng_), -180.0f);
			tier = 2;
		} else {
			// Hexagon
			type = EnemyType::HexagonBall;
			velocity = glm::vec2(dist_dir_(rng_), dist_dir_(rng_));
			tier = 1;
		}

		auto e = registry.CreateEntity();
		e.AddComponent<TransformComponent>(position, glm::vec2(1.0f), 0.0f);
		e.AddComponent<RigidbodyComponent>(velocity);
		e.AddComponent<EnemyComponent>(type, tier);
		e.AddComponent<SpriteComponent>("enemy-sprite", tier); // Replace with your texture logic
	}
};

#endif // ENEMY_SPAWN_SYSTEM_H_
