#version 430

in vec2 uv;

layout(location=4) uniform sampler2D diffuseTex;

layout(location=0) out vec4 fragColor;

void main()
{
	fragColor = texture(diffuseTex,uv);
}