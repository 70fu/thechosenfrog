#version 430

layout(location=0) in vec2 parabola;

out vec2 uvs;

//pass through vertex shader
void main()
{
	gl_Position = vec4(parabola.xy,0,1);
}