//Main Vertex Shader - Simple Diffuse + Ambient Lighting
#version 100
precision mediump float;

attribute 	vec3 	a_position;
attribute 	vec3 	a_normal;
attribute 	vec2 	a_texCoord;

uniform 	mat4 	u_MVP;

varying 	vec2 	v_texCoord;
varying 	vec3 	v_normal;

void main()
{
	gl_Position 	= u_MVP * vec4(a_position, 1);
	v_texCoord 		= a_texCoord;
	v_normal		= a_normal;
}