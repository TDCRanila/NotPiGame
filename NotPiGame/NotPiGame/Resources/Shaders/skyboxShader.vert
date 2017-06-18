//Skybox Vertex Shader
#version 100
precision mediump float;

attribute 	vec3 	a_position;

uniform 	mat4 	u_MVP;

varying		vec3	v_texCoord;

void main()
{
	vec4	pos		= u_MVP * vec4(a_position, 1);
	gl_Position		= pos.xyww;
	v_texCoord		= a_position;
}