#version 430

in vec2 uv;

layout(location=0) uniform sampler2D colorTexture;

layout(location=0) out vec4 fragColor;

void main()
{
	//fragColor = vec4(uv.y,uv.y,0.5,1);
	fragColor = vec4(texture(colorTexture,uv).rgb,1f);
}