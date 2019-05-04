#version 430

layout(location=0) in vec2 position;
layout(location=4) in vec2 uvIn;

out vec2 uvs;

//pass through vertex shader
void main()
{
	uvs=uvIn;

	gl_Position = vec4(position.xy,0,1);
}