#version 430
//taken from https://learnopengl.com/Advanced-OpenGL/Cubemaps

layout (location = 0) in vec3 position;

//layout(location=0) uniform mat4 model;
//layout(location=1) uniform mat4 normalMat;
layout(location=2) uniform mat4 pv;
//layout(location=3) uniform vec3 viewerPos;

out vec3 texCoords;

void main()
{
    texCoords = position;
    vec4 pos = pv * vec4(position, 1.0);
    gl_Position = pos.xyww;
}