#version 430

in vec2 uv;

layout(location=0) uniform sampler2D colorTexture;

layout(location=0) out vec4 fragColor;

void main()
{
	const ivec2 offsets[] = ivec2[]
	(
		ivec2(-1,1),ivec2(0,1),ivec2(1,1),
		ivec2(-1,0),ivec2(0,0),ivec2(1,0),
		ivec2(-1,-1),ivec2(0,-1),ivec2(1,-1)
	);

	const float kernel[] = float[]
	(
		1, 1, 1,
		1,-8, 1,
		1, 1, 1
	);

	ivec2 uvCoord = ivec2(floor(textureSize(colorTexture,0)*uv));
	vec3 color = vec3(0,0,0);
	for(int i = 0;i<offsets.length();++i)
	{
		color+=texelFetch(colorTexture,uvCoord+offsets[i],0).rgb*kernel[i];
	}

	//fragColor = vec4(texture(colorTexture,uv).rgb,1f);
	float gray =  0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	gray = 1-gray;
	fragColor = vec4(gray,gray,gray,1-gray);
}