#include "resource_manager.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

const std::string assetFolderPath{RESM_ASSET_PATH};

std::unordered_map<std::string, std::shared_ptr<Texture>> textures{};
std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms{};
std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes{};

std::string ResM::assetPathToFilePath(std::string assetPath)
{
	return assetFolderPath + assetPath;
}

std::shared_ptr<Texture> ResM::loadTexture(std::string assetPath, int req_comp)
{
	std::string fpath = assetPathToFilePath(assetPath);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(fpath.c_str(), &width, &height, &nrChannels, req_comp);
	auto tex = std::make_shared<Texture>();
	tex->load(data, width, height);
	textures.insert({assetPath, tex});
	stbi_image_free(data);
	return tex;
}

std::optional<std::shared_ptr<Texture>> ResM::getTexture(std::string assetPath)
{
	auto t = textures.find(assetPath);
	return t != textures.end() ? std::make_optional(t->second)
							   : std::nullopt;
}

std::shared_ptr<ShaderProgram> ResM::loadShaderProgram(std::string assetPath)
{
	std::string fpathV = assetPathToFilePath(assetPath + ".vert");
	std::string fpathF = assetPathToFilePath(assetPath + ".frag");

	std::ifstream tV(fpathV.c_str());
	std::stringstream sbufV;
	sbufV << tV.rdbuf();
	std::string srcV = sbufV.str();

	std::ifstream tF(fpathF.c_str());
	std::stringstream sbufF;
	sbufF << tF.rdbuf();
	std::string srcF = sbufF.str();

	auto shad = std::make_shared<ShaderProgram>();
	shad->load(srcV.c_str(), srcF.c_str());
	shaderPrograms.insert({assetPath, shad});

	return shad;
}

std::optional<std::shared_ptr<ShaderProgram>> ResM::getShaderProgram(std::string assetPath)
{
	auto t = shaderPrograms.find(assetPath);
	return t != shaderPrograms.end() ? std::make_optional(t->second)
							   : std::nullopt;
}

void ResM::destroyAll()
{
	// TODO: Implement ResM::destroyAll()
}