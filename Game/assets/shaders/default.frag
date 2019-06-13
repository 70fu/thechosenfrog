#version 430

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

void main()
{
	fragColor = vec4(1,0,0,1);
	normalOut = vec4(1,0,0,1);
}