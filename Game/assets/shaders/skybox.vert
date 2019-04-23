#version 430 core
layout (location = 0) in vec3 aPos;

layout(location=0) uniform mat4 model;
layout(location=1) uniform mat4 normalMat;
layout(location=2) uniform mat4 pv;
layout(location=3) uniform vec3 viewerPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}  