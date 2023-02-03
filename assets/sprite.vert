#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 fUV;

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
	fUV = aUV;
	// const vec4 vertices[3] = vec4[3](
    //     vec4(0.25, -0.25, 0.0, 1.0),
    //     vec4(-0.25, -0.25, 0.0, 1.0),
    //     vec4(0.25, 0.25, 0.0, 1.0)
    // );
    // gl_Position = vertices[gl_VertexID];
}