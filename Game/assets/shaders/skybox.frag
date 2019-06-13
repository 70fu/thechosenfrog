#version 430
//taken from https://learnopengl.com/Advanced-OpenGL/Cubemaps

in vec3 texCoords;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

layout(location=4) uniform samplerCube skybox;

void main()
{    
    fragColor = texture(skybox, texCoords);
    normalOut = vec4(0,0,0,1);
    //fragColor=vec4(texCoords,1);
}