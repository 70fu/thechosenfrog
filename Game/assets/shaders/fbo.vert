#version 430

layout(location=0) in vec3 position;
layout(location=4) in vec2 uvIn;

out vec2 uv;

void main()
{
	//forward uv and position
	uv = uvIn;
	gl_Position = vec4(position,1);
}