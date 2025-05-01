#ifndef GAME_RESET_SYSTEM_H_
#define GAME_RESET_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../components/tag_component.h"

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
			if (tag.tag == "player" || tag.tag == "enemy" || tag.tag == "ui_game")
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
	}
};

#endif // GAME_RESET_SYSTEM_H_
