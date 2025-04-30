target_sources( main PRIVATE
        "main.cpp"
        "engine/engine.h"
        "engine/engine.cpp"
        "logger/logger.h"
        "logger/logger.cpp"
        "asset_store/asset_store.h"
        "asset_store/asset_store.cpp"
        "ecs/ecs.h"
        "ecs/ecs.cpp"

        "components/transform_component.h"
        "components/rigidbody_component.h"
        "components/sprite_component.h"
        "components/input_component.h"
        "components/enemy_component.h"

        "systems/movement_system.h"
        "systems/render_system.h"
        "systems/input_system.h"
        "systems/player_control_system.h"
        "systems/enemy_control_system.h"
        "systems/enemy_spawn_system.h"

        "game/game_manager.h"
)
