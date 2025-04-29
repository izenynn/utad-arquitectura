#ifndef SPRITE_COMPONENT_H_
#define SPRITE_COMPONENT_H_

#include <string>
#include <cstddef>

#include <tigr.h>

struct SpriteComponent {
	std::string asset_id;
	size_t z_index;

	SpriteComponent(const std::string& asset_id = "", size_t z_index = 0)
		: asset_id(asset_id), z_index(z_index) {}
};

#endif // SPRITE_COMPONENT_H_
