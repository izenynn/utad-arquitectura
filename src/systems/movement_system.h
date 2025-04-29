#ifndef MOVEMENT_SYSTEM_H_
#define MOVEMENT_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/rigidbody_component.h"

class MovementSystem : public System {
public:
	MovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidbodyComponent>();
	}

	void Update(float delta_time)
	{
		for (const auto& entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

			transform.position += rigidbody.velocity * delta_time;
		}
	}
};

#endif // MOVEMENT_SYSTEM_H_
