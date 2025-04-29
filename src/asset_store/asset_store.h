#ifndef ASSET_STORE_H_
#define ASSET_STORE_H_

#include <string>
#include <map>

#include <tigr.h>

class AssetStore {
public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddImage(const std::string& id, const std::string& path);
	[[nodiscard]] Tigr* GetImage(const std::string& id) const;

private:
	std::map<std::string, Tigr*> images_;
	// TODO: create map for fonts
	// TODO: create map for audios
};

#endif // ASSET_STORE_H_
