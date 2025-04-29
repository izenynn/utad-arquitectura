#include "asset_store.h"

#include "../logger/logger.h"

AssetStore::AssetStore()
{
	Logger::Info("AssetStore constructor called");
}

AssetStore::~AssetStore()
{
	ClearAssets();
	Logger::Info("AssetStore destructor called");
}

void AssetStore::ClearAssets()
{
	for (auto& image : images_) {
		tigrFree(image.second);
	}
	images_.clear();
}

void AssetStore::AddImage(const std::string& id, const std::string& file_path)
{
	Tigr* img = tigrLoadImage(file_path.c_str());
	if (!img) {
		Logger::Error("Failed to load image: " + file_path);
		return;
	}

	images_.emplace(id, img);
	Logger::Info("AssetStore: Added image with id: " + id);
}

Tigr* AssetStore::GetImage(const std::string& id) const
{
	auto it = images_.find(id);
	if (it == images_.end())
		return nullptr;

	return it->second;
}
