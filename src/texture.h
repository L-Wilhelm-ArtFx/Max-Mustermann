#pragma once
#include <GL/glew.h>

class Texture
{
public:
public:
	GLuint texId = 0;
	int width, height;

public:
	Texture();

	bool load(unsigned char *data, int width, int height, GLint dataType = GL_RGBA);
	void use();
	void destroy();
	GLuint getTexId();
	int getWidth();
	int getHeight();
};