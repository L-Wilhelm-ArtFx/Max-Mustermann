#pragma once

#include <vector>
#include <memory>
#include <bitset>
#include <unordered_map>
#include <optional>
#include <yaml-cpp/yaml.h>

#include "texture.h"

typedef unsigned int tileId;

class TileClass
{
public:
	std::shared_ptr<Texture> texture;
	bool isSolid;

	TileClass(std::shared_ptr<Texture> texture, bool isSolid = true);
};

class TilePalette
{
public:
	std::unordered_map<tileId, std::shared_ptr<TileClass>> tileMap;

public:
	TilePalette();

	void setTileClass(tileId id, std::shared_ptr<TileClass> tileClass);

	std::weak_ptr<TileClass> getTileClass(tileId id);
};

struct TileSlot
{
	tileId type{0};
	TileSlot(tileId type = 0);
};

class TileMap
{
private:
	std::vector<TileSlot> tiles;
	unsigned int width, height;
	size_t nonAirTileCount;

public:
	TileMap(unsigned int width, unsigned int height);

	void fillAll(TileSlot tile);

	void setTile(unsigned int x, unsigned int y, TileSlot tile);

	TileSlot getTile(unsigned x, unsigned int y);

	void resize(unsigned int width, unsigned int height);

	size_t getNonAirTileCount();

	unsigned int getWidth();
	unsigned int getHeight();

	YAML::Node toYAML();
	void fromYAML(const YAML::Node& node);
};