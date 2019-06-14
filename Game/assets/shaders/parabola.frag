#version 430

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

in float alpha;
in vec3 viewNormal;

void main()
{
	fragColor = vec4(0,1,0,alpha);
	normalOut=vec4(normalize(viewNormal),1);
}