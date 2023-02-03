#version 440 core

layout (location = 0) in vec3 aPos;

uniform mat4 objectTransform;
uniform mat4 projection;
uniform mat4 viewTransform;
uniform mat4 transform;

void main()
{
	vec3 pos = aPos;
	// vec4 screenPos = perspective * viewTransform * objectTransform * vec4(pos.x, pos.y, pos.z, 1.0);
	vec4 screenPos = projection * viewTransform * transform * vec4(pos.x, pos.y, pos.z, 1.0);
	gl_Position = screenPos;
}