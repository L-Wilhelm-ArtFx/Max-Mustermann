#version 440 core

layout(origin_upper_left) in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec4 uCol1;
uniform vec4 uCol2;

void main()
{
	float checkerSize = 8.0;
	float checkered = round(mod(mod((floor(gl_FragCoord.x) + floor( gl_FragCoord.y)) / checkerSize, 1.0), 1.0));
	FragColor = mix(uCol1, uCol2, checkered);
}