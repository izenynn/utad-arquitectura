#ifndef SPRITE_COMPONENT_H_
#define SPRITE_COMPONENT_H_

#include <string>
#include <cstddef>

#include <tigr.h>

struct SpriteComponent {
	std::string asset_id;
	size_t width;
	size_t height;
	size_t z_index;
	Tigr* image;
	int src_x;
	int src_y;

	SpriteComponent(
		const std::string& asset_id = "",
		size_t w = 0, size_t h = 0,
		size_t z_index = 0,
		Tigr* image = nullptr,
		int src_x = 0, int src_y = 0)
		: asset_id(asset_id),
		  width(w),
		  height(h),
		  z_index(z_index),
		  image(image),
		  src_x(src_x),
		  src_y(src_y)
	{}
};

#endif // SPRITE_COMPONENT_H_
