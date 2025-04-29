#ifndef PLAYER_CONTROL_SYSTEM_H_
#define PLAYER_CONTROL_SYSTEM_H_

#include <tigr.h>

#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"
#include "../components/input_component.h"

class PlayerControlSystem : public System {
public:
	PlayerControlSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
		RequireComponent<InputComponent>();
	}

	void Update()
	{
		for (const auto& entity : GetSystemEntities()) {
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			auto& input = entity.GetComponent<InputComponent>();

			if (input.keys_held.count(TK_LEFT))
				rigidbody.velocity.x = -5.0f;
			if (input.keys_held.count(TK_RIGHT))
				rigidbody.velocity.x = 5.0f;
		}

	}
};

#endif // PLAYER_CONTROL_SYSTEM_H_
