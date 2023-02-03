#pragma once

#include <memory>
#include <string>
#include <optional>

#include "texture.h"
#include "shader.h"
#include "mesh.h"

#ifndef RESM_ASSET_PATH
#define RESM_ASSET_PATH "./assets/"
#endif

namespace ResM {

	std::string assetPathToFilePath(std::string assetPath);

	std::shared_ptr<Texture> loadTexture(std::string assetPath, int req_comp = 4);
	std::optional<std::shared_ptr<Texture>> getTexture(std::string assetPath);

	std::shared_ptr<ShaderProgram> loadShaderProgram(std::string assetPath);
	std::optional<std::shared_ptr<ShaderProgram>> getShaderProgram(std::string assetPath);

	void destroyAll();
}