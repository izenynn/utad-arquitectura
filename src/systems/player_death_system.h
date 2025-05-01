#ifndef PLAYER_DEATH_SYSTEM_H_
#define PLAYER_DEATH_SYSTEM_H_

#include <algorithm>
#include <functional>

#include <glm/glm.hpp>

#include "../asset_store/asset_store.h"
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/tag_component.h"
#include "../components/sprite_component.h"
#include "../systems/game_reset_system.h"
#include "../systems/enemy_spawn_system.h"

/*
 * PlayerDeathSystem checks for collisions between the player and enemies (balls/hexballs).
 * If a collision is detected, it triggers the on_player_death callback.
 * A full collision system with collider components would be too complex for this game.
 */
class PlayerDeathSystem : public System {
public:
	PlayerDeathSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<TagComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(Registry& registry, const AssetStore& asset_store)
	{
		const Entity* player = nullptr;
		glm::vec2 player_pos{};
		glm::vec2 player_size{};
		glm::vec2 player_pivot{0.5f, 0.5f};

		// Find player entity
		for (auto& entity : GetSystemEntities()) {
			auto& tag = entity.GetComponent<TagComponent>();
			if (tag.tag == "player") {
				player = &entity;
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				Tigr* image = asset_store.GetImage(sprite.asset_id);
				if (!image) {
					Logger::Error("Player Death System: Player image not found");
					continue;
				}

				player_pos = transform.position;
				player_pivot = transform.pivot;
				player_size = { image->w, image->h };
				break;
			}
		}

		// Could be in the menu :)
		if (!player) {
			return;
		}

		// Check for collisions with enemies
		const glm::vec2 player_min = player_pos - player_size * player_pivot;
		const glm::vec2 player_max = player_min + player_size;

		for (auto& entity : GetSystemEntities()) {
			auto& tag = entity.GetComponent<TagComponent>();
			if (tag.tag != "enemy")
				continue;

			auto& enemy_transform = entity.GetComponent<TransformComponent>();
			auto& enemy_sprite = entity.GetComponent<SpriteComponent>();
			Tigr* image = asset_store.GetImage(enemy_sprite.asset_id);
			if (!image) {
				Logger::Error("Player Death System: Enemy image not found");
				continue;
			}

			glm::vec2 circle_center = enemy_transform.position;
			float radius = static_cast<float>(image->w) / 2.0f;

			// Clamp circle center to rect bounds
			glm::vec2 closest_point = {
				std::clamp(circle_center.x, player_min.x, player_max.x),
				std::clamp(circle_center.y, player_min.y, player_max.y)
			};

			const float dist_sq = glm::dot(circle_center - closest_point, circle_center - closest_point);
			if (dist_sq <= radius * radius) {
				if (registry.HasSystem<GameResetSystem>())
					registry.GetSystem<GameResetSystem>().Run(registry);
				else
					Logger::Error("Player Death System: Game Reset System not found");

				if (registry.HasSystem<EnemySpawnSystem>())
					registry.GetSystem<EnemySpawnSystem>().Stop();
				else
					Logger::Error("Player Death System: Enemy Spawn System not found");

				return;
			}
		}
	}
};

#endif // PLAYER_DEATH_SYSTEM_H_
