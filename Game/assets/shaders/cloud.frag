#version 430

in vec3 viewNormal;
in vec4 color;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

void main()
{
	fragColor = color;
	normalOut = vec4(0,0,0,1);
	normalOut = vec4(normalize(viewNormal),1);
}