#ifndef GAME_START_SYSTEM_H_
#define GAME_START_SYSTEM_H_

#include <glm/glm.hpp>

#include "../ecs/ecs.h"

#include "../components/transform_component.h"
#include "../components/tag_component.h"
#include "../components/sprite_component.h"
#include "../components/text_component.h"
#include "../components/game_score_component.h"

#include "../systems/enemy_spawn_system.h"
#include "../systems/game_score_system.h"
#include "../systems/game_high_score_system.h"

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

		// Reset enemy spawn system
		if (registry.HasSystem<EnemySpawnSystem>())
			registry.GetSystem<EnemySpawnSystem>().Reset();

		// Reset score
		if (registry.HasSystem<GameScoreSystem>())
			registry.GetSystem<GameScoreSystem>().Reset();
		else
			Logger::Error("Game Start System: GameScoreSystem not found");
	}
};

#endif // GAME_START_SYSTEM_H_
