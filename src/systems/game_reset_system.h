#ifndef GAME_RESET_SYSTEM_H_
#define GAME_RESET_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../components/tag_component.h"
#include "../systems/enemy_spawn_system.h"
#include "../systems/game_score_system.h"
#include "../systems/game_high_score_system.h"

/*
 * GameRestartSystem handles the game restart process.
 * It removes all entities with the tags "player", "enemy", and "score".
 * And creates the start menu UI items.
 */
class GameResetSystem : public System {
public:
	GameResetSystem()
	{
		RequireComponent<TagComponent>();
	}

	void Run(Registry& registry)
	{
		const Entity* gm = nullptr;

		for (auto& entity : GetSystemEntities()) {
			auto& tag = entity.GetComponent<TagComponent>();

			// Remove player, enemies, and UI elements
			if (tag.tag == "player" || tag.tag == "enemy")
				registry.DestroyEntity(entity);

			// Find game_manager entity
			if (tag.tag == "game_manager")
				gm = &entity;
		}

		if (gm == nullptr) {
			Logger::Error("Game Reset System: Game Manager entity not found");
			return;
		}

		if (!gm->HasComponent<GameStateComponent>()) {
			Logger::Error("Game Reset System: GameStateComponent not found in Game Manager");
			return;
		}

		auto& game_state = gm->GetComponent<GameStateComponent>();
		game_state.state = GameState::Menu;

		Entity start_text = registry.CreateEntity();
		start_text.AddComponent<TransformComponent>(glm::vec2(50.0f, 100.0f));
		start_text.AddComponent<TagComponent>("ui_menu");
		start_text.AddComponent<TextComponent>("Press [SPACE] to start...", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Stop enemy spawn system
		if (registry.HasSystem<EnemySpawnSystem>())
			registry.GetSystem<EnemySpawnSystem>().Stop();
		else
			Logger::Error("Player Death System: Enemy Spawn System not found");

		// Stop score system
		if (registry.HasSystem<GameScoreSystem>())
			registry.GetSystem<GameScoreSystem>().Stop();
		else
			Logger::Error("Player Death System: Game Score System not found");

		// Save high score
		if (registry.HasSystem<GameScoreSystem>()) {
			if (registry.HasSystem<GameHighScoreSystem>())
				registry.GetSystem<GameHighScoreSystem>().SetHighScoreIfGreater(registry.GetSystem<GameScoreSystem>().GetScore());
			else
				Logger::Error("Game Reset System: GameHighScoreSystem not found");
		} else {
			Logger::Error("Game Reset System: GameScoreSystem not found");
		}

		// Load high score
		if (registry.HasSystem<GameHighScoreSystem>())
			registry.GetSystem<GameHighScoreSystem>().LoadHighScore();
		else
			Logger::Error("Game Start System: GameHighScoreSystem not found");
	}
};

#endif // GAME_RESET_SYSTEM_H_
