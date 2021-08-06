#pragma once
#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H
#include <string>
#include <vector>
#include <cstring>


namespace assets {
	struct AssetFile {
		char type[4];
		int version;
		std::string json;
		std::vector<char> binaryBlob;
	};

	enum class CompressionMode : uint32_t {
		None,
		LZ4
	};

	bool save_binaryfile(const char* path, const AssetFile& file);

	bool load_binaryfile(const char* path, AssetFile& outputFile);	

	assets::CompressionMode parse_compression(const char* f);
}

#endif