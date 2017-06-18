//Main Fragment Shader - Simple Diffuse + Ambient Lighting
#version 100
precision mediump float;

uniform sampler2D 	s_texture;
uniform vec4 		u_ambient;

varying vec2 		v_texCoord;
varying	vec3 		v_normal;

void main() 
{
	gl_FragColor = texture2D(s_texture, v_texCoord);
}