#include "texture.h"

Texture::Texture()
{
}

bool Texture::load(unsigned char *data, int width, int height, GLint dataType)
{
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, dataType, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	this->width = width;
	this->height = height;
	return true;
}

void Texture::use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void Texture::destroy()
{
	glDeleteTextures(1, &texId);
}

GLuint Texture::getTexId()
{
	return texId;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}