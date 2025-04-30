#ifndef ENEMY_CONTROL_SYSTEM_H_
#define ENEMY_CONTROL_SYSTEM_H_

#include <random>

#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/enemy_component.h"
#include "../components/sprite_component.h"

constexpr float padding = 6.0f;

class EnemyControlSystem : public System {
public:
	EnemyControlSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
		RequireComponent<EnemyComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(float delta_time, Registry& registry, const AssetStore& asset_store, int screen_w, int screen_h)
	{
		for (const auto& entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			auto& enemy = entity.GetComponent<EnemyComponent>();

			switch (enemy.type) {
				case EnemyType::BouncingBall:
					HandleBouncingBall(entity, delta_time, registry, asset_store, screen_w);
					break;
				case EnemyType::HexagonBall:
					HandleHexagonBall(transform, rigidbody, delta_time, static_cast<float>(screen_w), static_cast<float>(screen_h));
					break;
			}
		}
	}

private:
	void HandleBouncingBall(Entity entity, float delta_time, Registry& registry, const AssetStore& asset_store, int screen_w)
	{
		constexpr float gravity = 150.0f;
		constexpr float ground = 200.0f;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
		auto& enemy = entity.GetComponent<EnemyComponent>();

		rigidbody.velocity.y += gravity * delta_time;
		transform.position += rigidbody.velocity * delta_time;

		const float hh =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->h) / 2.0f;
		if (transform.position.y > ground - hh - padding) {
			transform.position.y = ground - hh - padding;
			rigidbody.velocity.y *= -0.99f;

			// Split logic
			if (enemy.tier > 1) {
				SpawnSplitBalls(entity, transform.position, enemy.tier - 1, registry);
				registry.DestroyEntity(entity);
			} else {
				registry.DestroyEntity(entity);
			}
		}

		// Left/right bounce
		const float hw =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->w) / 2.0f;
		if (transform.position.x <= 0.0f + hw + padding || transform.position.x >= static_cast<float>(screen_w) - hw - padding) {
			rigidbody.velocity.x *= -1;
		}
	}

	void SpawnSplitBalls(const Entity& source, glm::vec2 pos, int new_tier, Registry& registry)
	{
		(void)source;
		for (int i = 0; i < 2; ++i) {
			auto e = registry.CreateEntity();
			e.AddComponent<TransformComponent>(pos, glm::vec2(1.0f), 0.0f);
			e.AddComponent<RigidbodyComponent>(glm::vec2(i == 0 ? -50.0f : 50.0f, -150.0f));
			e.AddComponent<SpriteComponent>(std::string("ball") + std::to_string(new_tier), new_tier);
			e.AddComponent<EnemyComponent>(EnemyType::BouncingBall, new_tier);
		}
	}

	void HandleHexagonBall(TransformComponent& transform, RigidbodyComponent& rigidbody, float dt, float screen_w, float screen_h)
	{
		transform.position += rigidbody.velocity * dt;

		if (transform.position.x <= 0 || transform.position.x >= screen_w)
			rigidbody.velocity.x *= -1;

		if (transform.position.y <= 0 || transform.position.y >= screen_h)
			rigidbody.velocity.y *= -1;
	}
};

#endif // ENEMY_CONTROL_SYSTEM_H_
