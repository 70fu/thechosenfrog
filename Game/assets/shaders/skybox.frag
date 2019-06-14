#version 430
//taken from https://learnopengl.com/Advanced-OpenGL/Cubemaps

in vec3 texCoords;
in vec3 normal;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 normalOut;

layout(location=4) uniform samplerCube skybox;

void main()
{    
    fragColor = texture(skybox, texCoords);
    //normalOut = vec4(normalize(normal),1);
    normalOut=vec4(0,0,0,1);
    //fragColor=vec4(texCoords,1);
}