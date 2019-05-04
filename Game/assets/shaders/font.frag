#version 430

in vec2 uv;

layout(location=6) uniform sampler2D bitmap;
layout(location=7) uniform vec3 textColor;

layout(location=0) out vec4 fragColor;

void main()
{
	float alpha = texture(bitmap,uv).r;
	fragColor = vec4(textColor,texture(bitmap,uv).r);
}