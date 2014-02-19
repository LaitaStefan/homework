#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura;
uniform int has_alpha;

in vec3 color;
in vec2 texcoords;

void main(){

	vec3 tex = texture(textura, texcoords).xyz;
	
	if(has_alpha>0.5) if((tex.r>0.1) && (tex.g>0.1) && (tex.b>0.1)) discard;

	out_color = vec4(color,1);
}