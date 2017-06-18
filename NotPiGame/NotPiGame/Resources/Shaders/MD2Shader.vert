//Main Vertex Shader - Simple Diffuse + Ambient Lighting
#version 100
precision mediump float;

// Current Frame
attribute 	vec3 	a_position;
// Next Frame
attribute	vec3	a_position_next;
attribute 	vec3 	a_normal;
attribute 	vec2 	a_texCoord;

uniform 	mat4 	u_MVP;
uniform		float	u_animTime;

varying 	vec2 	v_texCoord;
varying 	vec3 	v_normal;

void main()
{
	// Calculating the interpolated vertex position	
	gl_Position 	= u_MVP * vec4( mix(a_position, a_position_next, u_animTime), 1);
	v_texCoord 		= a_texCoord;
	v_normal		= a_normal;
}