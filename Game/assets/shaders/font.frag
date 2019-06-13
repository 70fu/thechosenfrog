#version 430

in vec2 uv;
in vec3 viewNormal;

layout(location=6) uniform sampler2D bitmap;
layout(location=7) uniform vec3 textColor;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

void main()
{
	float alpha = texture(bitmap,uv).r;
	fragColor = vec4(textColor,texture(bitmap,uv).r);
	normalOut = vec4(normalize(abs(viewNormal)),1);
}