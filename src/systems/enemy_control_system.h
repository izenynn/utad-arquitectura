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
		(void)screen_h;
		for (const auto& entity : GetSystemEntities()) {
			switch (entity.GetComponent<EnemyComponent>().type) {
				case EnemyType::BouncingBall:
					HandleBouncingBall(entity, delta_time, registry, asset_store, screen_w);
					break;
				case EnemyType::HexagonBall:
					HandleHexagonBall(entity, delta_time, registry, asset_store, screen_w);
					break;
			}
		}
	}

private:
	void HandleBouncingBall(Entity entity, float delta_time, Registry& registry, const AssetStore& asset_store, int screen_w)
	{
		constexpr float gravity = 75.0f;
		constexpr float ground = 200.0f;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
		auto& enemy = entity.GetComponent<EnemyComponent>();

		rigidbody.velocity.y += gravity * delta_time;
		transform.position += rigidbody.velocity * delta_time;

		// Ground bounce
		const float hh =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->h) / 2.0f;
		if (transform.position.y > ground - hh - padding) {
			transform.position.y = ground - hh - padding;
			rigidbody.velocity.y *= -0.99f;

			// Split logic
			(void)registry;
			(void)enemy;
			if (enemy.tier > 1)
				SplitBouncingBall(entity, enemy.tier - 1, registry);
			else
				registry.DestroyEntity(entity);
		}

		// Left/right bounce
		const float hw =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->w) / 2.0f;
		if (transform.position.x <= 0.0f + hw + padding || transform.position.x >= static_cast<float>(screen_w) - hw - padding) {
			rigidbody.velocity.x *= -1;
		}
	}

	void HandleHexagonBall(Entity entity, float delta_time, Registry& registry, const AssetStore& asset_store, int screen_w)
	{
		constexpr float top = 8.0f;
		constexpr float ground = 200.0f;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
		auto& enemy = entity.GetComponent<EnemyComponent>();

		transform.position += rigidbody.velocity * delta_time;

		// Up/down bounce
		const float hh =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->h) / 2.0f;
		if (transform.position.y <= top + hh + padding || transform.position.y >= ground - hh - padding) {
			rigidbody.velocity.y *= -1;
			if (enemy.tier > 1)
				SplitHexagonBall(entity, enemy.tier - 1, registry, true);
			else
				registry.DestroyEntity(entity);
		}

		// Left/right bounce
		const float hw =  static_cast<float>(asset_store.GetImage(entity.GetComponent<SpriteComponent>().asset_id)->w) / 2.0f;
		if (transform.position.x <= 0.0f + hw + padding || transform.position.x >= static_cast<float>(screen_w) - hw - padding) {
			rigidbody.velocity.x *= -1;
			if (enemy.tier > 1)
				SplitHexagonBall(entity, enemy.tier - 1, registry, false);
			else
				registry.DestroyEntity(entity);
		}
	}

	void SplitBouncingBall(const Entity& source, int new_tier, Registry& registry)
	{
		auto& transform = source.GetComponent<TransformComponent>();
		auto& rigidbody = source.GetComponent<RigidbodyComponent>();

		for (int i = 0; i < 2; ++i) {
			auto e = registry.CreateEntity();
			e.AddComponent<TransformComponent>(transform.position);
			e.AddComponent<RigidbodyComponent>(glm::vec2(i == 0 ? -rigidbody.velocity.x : rigidbody.velocity.x, rigidbody.velocity.y));
			e.AddComponent<SpriteComponent>(std::string("ball") + std::to_string(new_tier), new_tier);
			e.AddComponent<EnemyComponent>(EnemyType::BouncingBall, new_tier);
			e.AddComponent<TagComponent>("enemy");
		}

		registry.DestroyEntity(source);
	}

	void SplitHexagonBall(const Entity& source, int new_tier, Registry& registry, bool is_vertical_collision)
	{
		auto& transform = source.GetComponent<TransformComponent>();
		auto& rigidbody = source.GetComponent<RigidbodyComponent>();

		for (int i = 0; i < 2; ++i) {
			auto e = registry.CreateEntity();
			e.AddComponent<TransformComponent>(transform.position);
			if (is_vertical_collision)
				e.AddComponent<RigidbodyComponent>(glm::vec2(i == 0 ? -rigidbody.velocity.x : rigidbody.velocity.x, rigidbody.velocity.y));
			else
				e.AddComponent<RigidbodyComponent>(glm::vec2(rigidbody.velocity.x, i == 0 ? -rigidbody.velocity.y : rigidbody.velocity.y));
			e.AddComponent<SpriteComponent>(std::string("hex") + std::to_string(new_tier), new_tier);
			e.AddComponent<EnemyComponent>(EnemyType::HexagonBall, new_tier);
			e.AddComponent<TagComponent>("enemy");
		}

		registry.DestroyEntity(source);
	}
};

#endif // ENEMY_CONTROL_SYSTEM_H_
