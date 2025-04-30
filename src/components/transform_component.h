#ifndef TRANSFORM_COMPONENT_H_
#define TRANSFORM_COMPONENT_H_

#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec2 position;
      	glm::vec2 scale;
        float rotation;
	glm::vec2 pivot;

        TransformComponent(
			glm::vec2 position = glm::vec2(0.0f, 0.0f),
       			glm::vec2 scale = glm::vec2(1.0f, 1.0f),
       			float rotation = 0.0f,
       			glm::vec2 pivot = glm::vec2(0.5f, 0.5f))
        		: position(position), scale(scale), rotation(rotation), pivot(pivot) {}
};

#endif // TRANSFORM_COMPONENT_H_
