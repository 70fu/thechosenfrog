#version 430

in vec2 uv;

layout(location=0) uniform sampler2D colorTexture;
layout(location=1) uniform float brightness=1;

layout(location=0) out vec4 fragColor;

void main()
{
	fragColor = vec4(texture(colorTexture,uv).rgb*brightness,1.0f);
}