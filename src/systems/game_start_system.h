#ifndef GAME_START_SYSTEM_H_
#define GAME_START_SYSTEM_H_

#include <glm/glm.hpp>

#include "../ecs/ecs.h"

#include "../components/transform_component.h"
#include "../components/tag_component.h"
#include "../components/sprite_component.h"
#include "../components/text_component.h"
#include "../components/score_component.h"

#include "../systems/enemy_spawn_system.h"

/*
 * GameStartSystem initializes the game when space is pressed.
 * It creates the player, UI elements, resets spawn, and starts the game state.
 */
class GameStartSystem : public System {
public:
	GameStartSystem()
	{
		RequireComponent<TagComponent>();
	}

	void Run(Registry& registry)
	{
		for (auto& entity : GetSystemEntities()) {
			auto& tag = entity.GetComponent<TagComponent>();

			// Destroy UI menu items
			if (tag.tag == "ui_menu")
				registry.DestroyEntity(entity);
		}

		// Create player entity
		Entity player = registry.CreateEntity();
		player.AddComponent<TagComponent>("player");
		player.AddComponent<TransformComponent>(glm::vec2(128.0f, 176.0f));
		player.AddComponent<RigidbodyComponent>();
		player.AddComponent<SpriteComponent>("player", 10);
		player.AddComponent<InputComponent>();

		// Create UI
		Entity time_text = registry.CreateEntity();
		time_text.AddComponent<TagComponent>("ui_game");
		time_text.AddComponent<TransformComponent>(glm::vec2(100.0f, 208.0f));
		time_text.AddComponent<TextComponent>("TIME", glm::vec4(0.9f, 0.9f, 0.0f, 1.0f));

		Entity time_score = registry.CreateEntity();
		time_score.AddComponent<TagComponent>("ui_game");
		time_score.AddComponent<TransformComponent>(glm::vec2(135.0f, 208.0f));
		time_score.AddComponent<TextComponent>("000", glm::vec4(0.9f, 0.5f, 0.1f, 1.0f));
		time_score.AddComponent<ScoreComponent>(0.0f);

		// Reset enemy spawn system
		if (registry.HasSystem<EnemySpawnSystem>())
			registry.GetSystem<EnemySpawnSystem>().Reset();
	}
};

#endif // GAME_START_SYSTEM_H_
