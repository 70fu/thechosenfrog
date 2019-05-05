#version 430

layout(location=0) out vec4 fragColor;

in float alpha;

void main()
{
	fragColor = vec4(0,1,0,alpha);
}