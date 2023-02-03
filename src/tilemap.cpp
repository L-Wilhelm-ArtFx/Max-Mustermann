#include "tilemap.h"

// TILECLASS

TileClass::TileClass(std::shared_ptr<Texture> texture, bool isSolid)
	: texture{texture},
	  isSolid{isSolid}
{
}

// TILEPALETTE

TilePalette::TilePalette() {}

void TilePalette::setTileClass(tileId id, std::shared_ptr<TileClass> tileClass)
{
	tileMap.insert_or_assign(id, tileClass);
}

std::weak_ptr<TileClass> TilePalette::getTileClass(tileId id)
{
	auto it = tileMap.find(id);
	return it == tileMap.end() ? std::weak_ptr<TileClass>()
							   : it->second;
}

// TILESLOT

TileSlot::TileSlot(tileId type)
	: type{type}
{
}

// TILEMAP

TileMap::TileMap(unsigned int width, unsigned int height)
{
	size_t tileCount = (size_t)width * (size_t)height;
	tiles.reserve(tileCount);
	for (size_t i = 0; i < tileCount; ++i)
	{
		tiles.emplace_back();
	}
	this->width = width;
	this->height = height;
}

void TileMap::fillAll(TileSlot tile)
{
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		*it = tile;
	}
	nonAirTileCount = tile.type == 0 ? 0 : tiles.size();
}

void TileMap::setTile(unsigned int x, unsigned int y, TileSlot tile)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return;
	size_t i = (size_t)y * width + (size_t)x;
	auto old = tiles[i];
	tiles[i] = tile;
	bool wasAir = old.type == 0;
	bool isAir = tile.type == 0;
	if (wasAir != isAir)
	{
		nonAirTileCount += isAir ? -1 : 1;
	}
}

TileSlot TileMap::getTile(unsigned x, unsigned int y)
{
	bool rangeInvalid = x < 0 || y < 0 || x >= width || y >= height;
	return rangeInvalid ? TileSlot()
						: tiles[x + y * width];
}

void TileMap::resize(unsigned int newWidth, unsigned int newHeight)
{
	auto oldTiles = tiles;
	size_t tileCount = (size_t)newWidth * (size_t)newHeight;
	tiles.resize(tileCount, TileSlot(0U));

	for (size_t i = 0; i < tileCount; ++i)
	{
		size_t x = i % newWidth;
		size_t y = i / newWidth;

		if (x < width && y < height)
		{
			size_t oldI = x + y * width;
			tiles[i] = oldTiles[oldI];
		}
		else
		{
			tiles[i] = TileSlot(0U);
		}
	}

	width = newWidth;
	height = newHeight;
}

size_t TileMap::getNonAirTileCount()
{
	return nonAirTileCount;
}

unsigned int TileMap::getWidth()
{
	return width;
}
unsigned int TileMap::getHeight()
{
	return height;
}

YAML::Node TileMap::toYAML() {
	YAML::Node node;
	node["width"] = width;
	node["height"] = height;
	std::vector<tileId> t{};
	t.reserve(tiles.size());
	
	for(auto it = tiles.begin(); it != tiles.end(); ++it){
		t.push_back(it->type);
	}
	node["tiles"] = t;
	return node;
}

void TileMap::fromYAML(const YAML::Node& node){
	width = node["width"].as<unsigned int>();
	height = node["height"].as<unsigned int>();
	tiles.resize(width * height);
	
	auto tilesNode = node["tiles"];
	auto t = tilesNode.as<std::vector<unsigned int>>();
	for(size_t i = 0;i < tiles.size(); ++i){
		tiles[i] = i < t.size() ? TileSlot(t[i]) : TileSlot(0);
	}
}