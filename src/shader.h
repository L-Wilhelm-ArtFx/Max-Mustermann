#pragma once
#include <GL/glew.h>

class ShaderProgram
{
public:
private:
	GLuint progId = 0;

public:
	ShaderProgram();

	bool load(
		const GLchar *vertexSrc,
		const GLchar *fragmentSrc,
		const GLchar *tessControlSrc = nullptr,
		const GLchar *tessEvalSrc = nullptr,
		const GLchar *geometrySrc = nullptr);
	void use();
	void destroy();
	GLuint getProgramId();

private:
	GLuint createShader(const GLchar *src, GLenum shaderType);
};