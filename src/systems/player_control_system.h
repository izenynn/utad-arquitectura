#ifndef PLAYER_CONTROL_SYSTEM_H_
#define PLAYER_CONTROL_SYSTEM_H_

#include <tigr.h>

#include "../ecs/ecs.h"
#include "../asset_store/asset_store.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/input_component.h"

class PlayerControlSystem : public System {
public:
	PlayerControlSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<InputComponent>();
	}

	void Update(float delta_time)
	{
		constexpr float speed = 128.0f;

		for (const auto& entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			auto& input = entity.GetComponent<InputComponent>();

			// Set the player's velocity based on input
			const bool left = input.keys_held.count(TK_LEFT);
			const bool right = input.keys_held.count(TK_RIGHT);
			if (left && !right)
				rigidbody.velocity.x = -speed;
			else if (right && !left)
				rigidbody.velocity.x = speed;
			else
				rigidbody.velocity.x = 0.0f;

			// Clamp the player's position to the screen bounds
			if (transform.position.x + rigidbody.velocity.x * delta_time < 20.0f) {
				transform.position.x = 20.0f;
				rigidbody.velocity.x = 0.0f;
			} else if (transform.position.x + rigidbody.velocity.x * delta_time > 236.0f) {
				transform.position.x = 236.0f;
				rigidbody.velocity.x = 0.0f;
			}
		}
	}
};

#endif // PLAYER_CONTROL_SYSTEM_H_
