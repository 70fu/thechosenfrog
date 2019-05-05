#version 430

//normalized x value to substitute in the parabola formula
layout(location=0) in float x;

layout(location=16) uniform float maxX;//the parabola is calculated until this x value
layout(location=17) uniform float distance;//at maxX, this distance has been covered
layout(location=18) uniform vec2 parabola;//x = a, y = b

//the vertex shader evaluates the parabola formula and sends it to the geometry shader
void main()
{
	gl_Position = vec4(x);

	//normalized x -> x
	/*float pX = x*maxX;
	gl_Position = vec4(x*distance,parabola.x*pX*pX+parabola.y*pX,0,1);
	tangents = normalize(vec2(1,2*parabola.x*pX+parabola.y));*/
}