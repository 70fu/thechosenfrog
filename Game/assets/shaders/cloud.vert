#version 430

layout(location=0) in vec3 vertexPosition;
layout(location=2) in vec3 normalIn;
layout(location=4) in vec2 uvIn;
layout(location=6) in vec3 spherePosition;
layout(location=7) in vec3 scale;
layout(location=8) in vec3 offset;
layout(location=9) in vec4 colorIn;
layout(location=10) in mat4 normalViewModel;

layout(location=2) uniform mat4 pv;

out vec3 viewNormal;
out vec4 color;

void main()
{
	const float NORMAL_THRESHOLD = 0.4;

	color = colorIn;
	if(abs(normalIn.y)>NORMAL_THRESHOLD)
	{
		viewNormal=vec3(0,0,0);
	}
	else
	{
		viewNormal=vec3(normalViewModel * vec4(normalIn,0));
	}

	//1. Scale sphere by given scale
	//2. move vertex to sphere position
	//3. Translate by scaled offset
	vec3 position = vertexPosition*scale+spherePosition+offset*scale;
	gl_Position = pv * vec4(position,1);
}