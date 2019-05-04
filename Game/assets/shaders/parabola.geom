#version 430
#define NUM_SEGMENTS_PLUS_ONE 128

layout (points) in;
layout (line_strip, max_vertices = NUM_SEGMENTS_PLUS_ONE) out;

layout(location=0) uniform mat4 model;
layout(location=2) uniform mat4 pv;
layout(location=16) uniform float maxX;//the parabola is calculated until this x value
layout(location=17) uniform float distance;//at maxX, this distance has been covered

//emit a quad consisting of two triangles from a line (top left to bottom right)
void main() {
    mat4 pvm = pv * model;
    float step = maxX/(NUM_SEGMENTS_PLUS_ONE-1);
    vec4 parabola = gl_in[0].gl_Position;
    float distanceScale = (distance/maxX);//dividing through maxX scales x from 0 to 1

    //calculate segments
    for(int i = 0;i<NUM_SEGMENTS_PLUS_ONE;++i)
    {
        float x = step*i;
        float y = parabola.x*x*x+parabola.y*x;//there is no c component

        //construct point relative to origin, but use x as z value (-> jump is performed forwards)
        vec4 position = vec4(0.25,y-0.75,-x*distanceScale+0.25,1);//0.25 is the radius of the player, 0.75 is the footheight //TODO remove these hard coded values

        //transform and emit
        gl_Position = pvm*position;
        EmitVertex();
    }

    EndPrimitive();
}