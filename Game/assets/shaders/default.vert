#version 430

layout(location=0) in vec3 position;

layout(location=0) uniform mat4 model;
layout(location=1) uniform mat4 normalMat;
layout(location=2) uniform mat4 pv;
layout(location=3) uniform vec3 viewerPos;
layout(location=5) uniform mat4 normalModelView;

void main()
{
	gl_Position = pv * model * vec4(position,1);
}