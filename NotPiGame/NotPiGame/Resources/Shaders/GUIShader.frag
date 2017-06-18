//GUI Fragment Shader - Simple Diffuse + Ambient Lighting
#version 100
precision mediump float;

uniform sampler2D 	s_texture;
uniform vec4 		u_ambient;

varying vec2 		v_texCoord;
varying	vec3 		v_normal;

void main() 
{
	vec4 texColour 	= texture2D(s_texture, v_texCoord);
	// Discard the frag when it has an alpha smaller then 0.5f
	if (texColour.a < 0.5) {
		discard;
	}
	
	gl_FragColor 	= vec4(texColour.rgb, 1); 
}