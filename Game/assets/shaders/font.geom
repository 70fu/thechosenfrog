#version 430
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

layout(location=0) uniform mat4 model;
layout(location=1) uniform mat4 viewMatrices[2];//identity matrix when in screen space (0), view matrix when in world space (1), be aware that this takes up two locations
layout(location=3) uniform mat4 projMatrices[2];//orthogonal matrix when in screen space (0), normal perspective matrix when in world space (1), takes 2 uniform locations
layout(location=5) uniform int inWorldSpace;//0 if in screenspace,1 for world space

in vec2 uvs[];
out vec2 uv;

//emit a quad consisting of two triangles from a line (top left to bottom right)
void main() {
    mat4 pvm = projMatrices[inWorldSpace] * viewMatrices[inWorldSpace] * model;
    vec4 topLeft = gl_in[0].gl_Position;
    topLeft.y*=-1;
    vec4 bottomRight = gl_in[1].gl_Position;
    bottomRight.y*=-1;

    //top left
    gl_Position = pvm * topLeft;
    uv=uvs[0];
    //uv=vec2(0.16796875,0.28125);
    EmitVertex();

    //bottom left
    gl_Position = pvm * vec4(topLeft.x,bottomRight.y,0,1);
    uv=vec2(uvs[0].x,uvs[1].y);
    //uv=vec2(0.16796875,0.421875);
    EmitVertex();

    //top right
    gl_Position = pvm * vec4(bottomRight.x,topLeft.y,0,1);
    uv=vec2(uvs[1].x,uvs[0].y);
    //uv=vec2(0.21875,0.28125);
    EmitVertex();

    //bottom right
    gl_Position = pvm * bottomRight;
    uv=uvs[1];
    //uv=vec2(0.21875,0.421875);
    EmitVertex();

    EndPrimitive();
}