#version 430

layout(location=0) in vec3 position;
layout(location=2) in vec3 normalIn;
layout(location=4) in vec2 uvIn;

layout(location=0) uniform mat4 model;
layout(location=1) uniform mat4 normalMat;
layout(location=2) uniform mat4 pv;
layout(location=3) uniform vec3 viewerPos;
layout(location=5) uniform mat4 normalModelView;

out vec3 viewNormal;
out vec2 uv;

void main()
{
	uv=uvIn;
	viewNormal=vec3(normalModelView * vec4(normalIn,0));

	gl_Position = pv * model * vec4(position,1);
}