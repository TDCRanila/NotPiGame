//Skybox Fragment Shader
#version 100
precision mediump float;

uniform samplerCube s_cubeMap;

varying		vec3	v_texCoord;

void main() 
{
	gl_FragColor = textureCube(s_cubeMap, v_texCoord);
}