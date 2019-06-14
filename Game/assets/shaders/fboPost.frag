#version 430

in vec2 uv;
in vec3 position;

layout(location=0) uniform float brightness=1;
layout(location=1) uniform sampler2D normalTexture;
layout(location=2) uniform int normalSampleSize=1;
layout(location=3) uniform float normalThreshold;
layout(location=4) uniform vec4 normalOutlineColor;
layout(location=5) uniform sampler2D depthTexture;
layout(location=6) uniform int depthSampleSize=1;
layout(location=7) uniform float depthThreshold;
layout(location=8) uniform vec4 depthOutlineColor;

layout(location=0) out vec4 fragColor;

const float near=0.001;
const float far=100;

//taken from https://learnopengl.com/Advanced-OpenGL/Depth-testing
float linearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}

//returns difference between the two given texels 2-1
vec3 diff(sampler2D tex, ivec2 texel1,ivec2 texel2) {
	return texelFetch(tex,texel2,0).rgb-texelFetch(tex,texel1,0).rgb;
}

float dDiff(sampler2D tex, ivec2 texel1,ivec2 texel2) {
	return linearizeDepth(texelFetch(tex,texel2,0).r)-linearizeDepth(texelFetch(tex,texel1,0).r);
}

vec3[2] diffValues(sampler2D tex,int sampleSize)
{
	int halfWidthCeil = int(ceil(sampleSize*0.5));
	int halfWidthFloor = int(floor(sampleSize*0.5));

	//generate texture texel coords to sample from
	ivec2 size = textureSize(tex,0);
	ivec2 uvCoord = ivec2(floor(textureSize(tex,0)*uv));
	ivec2 bl = uvCoord+ivec2(-1,-1)*halfWidthFloor;
	ivec2 tr = uvCoord+ivec2(1,1)*halfWidthCeil;
	ivec2 tl = uvCoord+ivec2(-halfWidthFloor,halfWidthCeil);
	ivec2 br = uvCoord+ivec2(halfWidthCeil,-halfWidthFloor);

	return vec3[2](diff(tex,bl,tr),diff(tex,tl,br));
}

float[2] dDiffValues(sampler2D tex,int sampleSize)
{
	int halfWidthCeil = int(ceil(sampleSize*0.5));
	int halfWidthFloor = int(floor(sampleSize*0.5));

	//generate texture texel coords to sample from
	ivec2 size = textureSize(tex,0);
	ivec2 uvCoord = ivec2(floor(textureSize(tex,0)*uv));
	ivec2 bl = uvCoord+ivec2(-1,-1)*halfWidthFloor;
	ivec2 tr = uvCoord+ivec2(1,1)*halfWidthCeil;
	ivec2 tl = uvCoord+ivec2(-halfWidthFloor,halfWidthCeil);
	ivec2 br = uvCoord+ivec2(halfWidthCeil,-halfWidthFloor);

	return float[2](dDiff(tex,bl,tr),dDiff(tex,tl,br));
}

void main()
{
	//inspired by https://roystan.net/articles/outline-shader.html
	//and https://gamedev.stackexchange.com/questions/68401/how-can-i-draw-outlines-around-3d-models

	vec3[2] diffs = diffValues(normalTexture,normalSampleSize);
	float nEdge = sqrt(dot(diffs[0], diffs[0])+dot(diffs[1], diffs[1]));
	nEdge = nEdge>=normalThreshold?1:0;

	//calculate normal edge value if there is no depth edge
	float dEdge = 0;
	if(nEdge<1)
	{
		//calculate depth edge value
		float[2] dDiffs = dDiffValues(depthTexture,depthSampleSize);
		dEdge = sqrt(pow(dDiffs[0], 2) + pow(dDiffs[1], 2));

		vec3 normal = texture(normalTexture,uv).rgb;
		float normalDepthThresholdMultiplier = 2*clamp(((1-normal.z) - 0.5) / (1 - 0.5),0,1)+1;
		float modulatedThreshold = depthThreshold*normalDepthThresholdMultiplier;
	//TODO far away objects require bigger threshold
		dEdge = dEdge>=modulatedThreshold?1:0;
	}

	vec4 edgeColor = dEdge*depthOutlineColor + nEdge*normalOutlineColor;

	fragColor = vec4(edgeColor.rgb*brightness,edgeColor.a);

	//debug outputs
	//fragColor=vec4(nEdge,nEdge,nEdge,1);
	//fragColor=vec4(vec3((dEdge+nEdge)*brightness),1);
	//fragColor=vec4(texture(normalTexture,uv).zzz,1);
	//fragColor=vec4(vec3(dotViewNormal),1);
	//fragColor=vec4(vec3(position.x,-position.y,position.z),1);
}

/* Old kernel method
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
*/