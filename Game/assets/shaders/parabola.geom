#version 430

const int patchSegments = 8;
//these values are all relative (1=100% of parabola)
//const float patchGap = 0.01;
const float patchLength=1.0/64.0;
const float segmentLength = patchLength/patchSegments;
//these values are absolute values (in units)
const float patchHalfThickness = 0.025;
const float patchHalfWidth = 0.05;

//duration of animation in frame count
const int animDuration = 40;//2/3 s
const float animDistance = 2*patchLength;//TODO replace hard coded 2 (2 because the gap between patches is exactly one patch big)

//alpha transition, further away patches are less transparent
out float alpha;
const float minAlpha=0.1;
const float maxAlpha=0.75;

layout (points) in;
layout (triangle_strip, max_vertices = 70) out;//amount of vertices=(patchSegments+1)*4*2-2

layout(location=0) uniform mat4 model;
layout(location=2) uniform mat4 pv;
layout(location=4) uniform int frame;
layout(location=16) uniform float maxX;//the parabola is calculated until this x value
layout(location=17) uniform float distance;//at maxX, this distance has been covered
layout(location=18) uniform vec2 parabola;//x = a, y = b

//interpolation formula taken from https://github.com/Michaelangel007/easing#cleanup---out-quartic
float easeOutQuart(float t)
{
    t=t-1;
    return 1-t*t*t*t;
}

//pX is not normalized, thus should be between 0 and maxX
//returns normalized tangent at point x
vec2 pTangent(float pX)
{
    return normalize(vec2(1,2*parabola.x*pX+parabola.y));
}

//pX is not normalized, thus should be between 0 and maxX
//returns normalized normal at point x pointing outwards
vec2 pNormal(float pX)
{
    vec2 tangent = pTangent(pX);
    return vec2(-tangent.y,tangent.x);
}

//pX is not normalized, thus should be between 0 and maxX
float p(float pX)
{
    return parabola.x*pX*pX+parabola.y*pX;
}

//emit a quad consisting of two triangles from a line (top left to bottom right)
void main() {
    //frame -> animation time (0-1)
    float time = float((frame%animDuration))/float(animDuration);

    //get x value to substitute in the parabola formula
    float x = gl_in[0].gl_Position.x + time*animDistance;
    float pX = x*maxX;

    //set alpha for patch, quadratic interpolation //TODO other interpolation might be more suitable?
    alpha = (maxAlpha-minAlpha)*easeOutQuart(x)+minAlpha;

    //calculate segment points and normals (pointing outside)
    vec4 segmentPoints[patchSegments+1];
    vec4 segmentNormals[patchSegments+1];//scaled by halfThickness
    vec4 offset = vec4(0.25,-0.75,0.25,0);//0.25 is the radius of the player, 0.75 is the footheight //TODO remove these hard coded values
    for(int i = 0;i<=patchSegments;++i)
    {
        float segmentX = pX+segmentLength*i*maxX;
        segmentPoints[i]=vec4(0,p(segmentX),-(x+segmentLength*i)*distance,1)+offset;
        vec2 normal = pNormal(segmentX)*patchHalfThickness;
        segmentNormals[i]=vec4(0,normal.y,-normal.x,0);
    }

    //create triangle strip
    mat4 pvm = pv * model;
    vec4 halfWidth = vec4(patchHalfWidth,0,0,0);
    //upper side
    for(int i = 0;i<=patchSegments;++i)
    {
        gl_Position = pvm * (segmentPoints[i]-halfWidth+segmentNormals[i]);
        EmitVertex();
        gl_Position = pvm * (segmentPoints[i]+halfWidth+segmentNormals[i]);
        EmitVertex();
    }

    //transition to right side
    gl_Position = pvm * (segmentPoints[patchSegments]+halfWidth-segmentNormals[patchSegments]);
    EmitVertex();

    //right side
    for(int i = patchSegments-1;i>=0;--i)
    {
        gl_Position = pvm * (segmentPoints[i]+halfWidth+segmentNormals[i]);
        EmitVertex();
        gl_Position = pvm * (segmentPoints[i]+halfWidth-segmentNormals[i]);
        EmitVertex();
    }

    //complete front side
    gl_Position = pvm * (segmentPoints[0]-halfWidth+segmentNormals[0]);
    EmitVertex();
    gl_Position = pvm * (segmentPoints[0]-halfWidth-segmentNormals[0]);
    EmitVertex();

    //left side
    for(int i = 1;i<=patchSegments;++i)
    {
        gl_Position = pvm * (segmentPoints[i]-halfWidth+segmentNormals[i]);
        EmitVertex();
        gl_Position = pvm * (segmentPoints[i]-halfWidth-segmentNormals[i]);
        EmitVertex();
    }

    //transition to bottom side, complete back side
    gl_Position = pvm * (segmentPoints[patchSegments]+halfWidth-segmentNormals[patchSegments]);
    EmitVertex();

    //bottom side
    for(int i = patchSegments-1;i>=0;--i)
    {
        gl_Position = pvm * (segmentPoints[i]-halfWidth-segmentNormals[i]);
        EmitVertex();
        gl_Position = pvm * (segmentPoints[i]+halfWidth-segmentNormals[i]);
        EmitVertex();
    }

    EndPrimitive();
}