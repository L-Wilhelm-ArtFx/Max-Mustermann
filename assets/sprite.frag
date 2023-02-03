#version 440 core

in vec2 fUV;

out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D texSampler;

void main()
{
	vec4 color = uColor;
	FragColor = texture(texSampler, fUV);
}