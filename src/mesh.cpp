#include "mesh.h"

#include <iostream>

GLuint Mesh::getVaoID()
{
	return vao;
}

// SpriteMesh

SpriteMesh::SpriteMesh()
{
}

SpriteMesh::~SpriteMesh()
{
	Mesh::~Mesh();
}

void SpriteMesh::load(const std::vector<float> &verts, const std::vector<unsigned int> &indices)
{
	destroy();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertCount = indices.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void SpriteMesh::use()
{
	glBindVertexArray(vao);
}

void SpriteMesh::draw()
{
	use();
	glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);
}

void SpriteMesh::destroy()
{
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
	}
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	if (ebo)
	{
		glDeleteBuffers(1, &ebo);
	}
	vertCount = 0;
}

unsigned int SpriteMesh::getVertCount()
{
	return vertCount;
}

// ShapeMesh

ShapeMesh::ShapeMesh()
{
}

ShapeMesh::~ShapeMesh()
{
	Mesh::~Mesh();
}

void ShapeMesh::load(const std::vector<float> &verts)
{
	destroy();
	vertCount = verts.size() / 3;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void ShapeMesh::use()
{
	glBindVertexArray(vao);
}

void ShapeMesh::draw()
{
	use();
	glDrawArrays(GL_LINES, 0, vertCount);
}

void ShapeMesh::destroy()
{
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
	}
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	vertCount = 0;
}

unsigned int ShapeMesh::getVertCount()
{
	return vertCount;
}

// TileMapMesh

// TileMapMesh::TileMapMesh()
// {
// }

// TileMapMesh::~TileMapMesh()
// {
// 	Mesh::~Mesh();
// }

// void TileMapMesh::load(const std::vector<float> &verts, const std::vector<unsigned int> &indices)
// {
// }

// void TileMapMesh::use()
// {
// 	glBindVertexArray(vao);
// }

// void TileMapMesh::draw()
// {
// 	use();
// 	glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);
// }

// void TileMapMesh::destroy()
// {
// }

// unsigned int TileMapMesh::getVertCount()
// {
// 	return vertCount;
// }
