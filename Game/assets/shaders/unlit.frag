#version 430

in vec2 uv;


layout(location=16) uniform sampler2D diffuseTex;
layout(location=17) uniform vec2 texRepeat;
layout(location=18) uniform vec4 colorMultiply=vec4(1,1,1,1);

layout(location=0) out vec4 fragColor;

void main()
{
	fragColor = texture(diffuseTex,uv*texRepeat)*colorMultiply;
}