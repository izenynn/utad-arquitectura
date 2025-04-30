#ifndef RENDER_SYSTEM_H_
#define RENDER_SYSTEM_H_

#include <algorithm>
#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/sprite_component.h"
#include "../asset_store/asset_store.h"

class RenderSystem : public System {
public:
	RenderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(Tigr* window, std::unique_ptr<AssetStore>& asset_store)
	{
		struct RenderableEntity {
			TransformComponent transform;
			SpriteComponent sprite;
		};
		std::vector<RenderableEntity> renderable_entities;

		for (auto& entity : GetSystemEntities()) {
			renderable_entities.push_back({
				entity.GetComponent<TransformComponent>(),
				entity.GetComponent<SpriteComponent>()
			});
		}

		std::sort(
			renderable_entities.begin(),
			renderable_entities.end(),
			[](const RenderableEntity& a, const RenderableEntity& b) {
				return a.sprite.z_index < b.sprite.z_index;
			}
		);

		for (const auto& entity : renderable_entities) {
			const auto& transform = entity.transform;
			const auto& sprite = entity.sprite;

			Tigr* img = asset_store->GetImage(sprite.asset_id);
			if (!img) continue;

			int dest_x = std::max(0, static_cast<int>(transform.position.x - static_cast<float>(img->w) * transform.pivot.x));
			int dest_y = std::max(0, static_cast<int>(transform.position.y - static_cast<float>(img->h) * transform.pivot.y));

			// Basic blit (no scale/rotation support in Tigr)
			tigrBlitAlpha(window, img, dest_x, dest_y, 0, 0, img->w, img->h, 1.0f);
		}
	}
};

#endif // RENDER_SYSTEM_H_
