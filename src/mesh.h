#pragma once

#include <vector>
#include <GL/glew.h>

#include "tilemap.h"

class Mesh
{
protected:
	GLuint vao = 0;

public:
	Mesh() = default;
	Mesh(const Mesh &) = delete;

	// virtual void load(const std::vector<float> &verts, const std::vector<unsigned int> &indices) = 0;

	virtual void use() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
	GLuint getVaoID();
	virtual unsigned int getVertCount() = 0;
};

class SpriteMesh : public Mesh
{
protected:
	GLuint vbo = 0;
	GLuint ebo = 0;
	unsigned int vertCount = 0;

public:
	SpriteMesh();
	~SpriteMesh();

	void load(const std::vector<float> &verts, const std::vector<unsigned int> &indices);

	virtual void use() override;
	virtual void draw() override;
	virtual void destroy() override;

	virtual unsigned int getVertCount() override;
};

class ShapeMesh : public Mesh
{
protected:
	GLuint vbo = 0;
	unsigned int vertCount = 0;

public:
	ShapeMesh();
	~ShapeMesh();

	void load(const std::vector<float> &verts);

	virtual void use() override;
	virtual void draw() override;
	virtual void destroy() override;

	virtual unsigned int getVertCount() override;
};

// class TileMapMesh : public Mesh {
// 	protected:
// 	GLuint vbo = 0;
// 	GLuint ebo = 0;
// 	unsigned int vertCount = 0;

// public:
// 	TileMapMesh();
// 	~TileMapMesh();

// 	void load(TileMap &map);

// 	virtual void use() override;
// 	virtual void draw() override;
// 	virtual void destroy() override;

// 	virtual unsigned int getVertCount() override;
// };