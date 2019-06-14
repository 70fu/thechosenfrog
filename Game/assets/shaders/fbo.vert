#version 430

layout(location=0) in vec3 positionIn;
layout(location=4) in vec2 uvIn;

out vec2 uv;
out vec3 position;

void main()
{
	//forward uv and position
	uv = uvIn;
	position = positionIn;
	gl_Position = vec4(positionIn,1);
}