#version 430

layout(location=0) in vec3 position;

layout(location=0) uniform mat4 pv;
layout(location=1) uniform mat4 model;

void main()
{
	gl_Position = pv * model * vec4(position,1);
}