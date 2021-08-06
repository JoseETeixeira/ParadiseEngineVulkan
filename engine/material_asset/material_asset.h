#pragma once
#ifndef MATERIAL_ASSET_H
#define MATERIAL_ASSET_H
#include "../asset_loader/asset_loader.h"
#include <unordered_map>

namespace assets {

	enum class TransparencyMode:uint8_t {
		Opaque,
		Transparent,
		Masked
	};

	struct MaterialInfo {
		std::string baseEffect;
		std::unordered_map<std::string, std::string> textures; //name -> path
		std::unordered_map<std::string, std::string> customProperties;
		TransparencyMode transparency;
	};

	MaterialInfo read_material_info(AssetFile* file);

	AssetFile pack_material(MaterialInfo* info);
}

#endif