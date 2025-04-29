#ifndef RIGIDBODY_COMPONENT_H_
#define RIGIDBODY_COMPONENT_H_

#include <glm/glm.hpp>

struct RigidbodyComponent {
      	glm::vec2 velocity;

	RigidbodyComponent(glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
		: velocity(velocity) {}
};

#endif // RIGIDBODY_COMPONENT_H_
