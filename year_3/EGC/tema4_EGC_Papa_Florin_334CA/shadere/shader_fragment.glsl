#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform int has_alpha;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_ks;

in vec2 texcoord;
in vec3 world_pos;
in vec3 world_normal;

void main(){

	vec3 tex1 = texture(textura1, texcoord).xyz;
	vec3 tex2 = texture(textura2, texcoord).xyz;

	float light=0;

	vec3 L = normalize(light_position - world_pos);
	vec3  V  =  normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);
	tex1.x = tex1.x * 0.2 + tex1.x * max(0, dot(L, world_normal)) + material_ks*pow(max(0, dot(H, world_normal)), material_shininess);
	tex1.y = tex1.y * 0.2 + tex1.y * max(0, dot(L, world_normal)) + material_ks*pow(max(0, dot(H, world_normal)), material_shininess);
	tex1.z = tex1.z * 0.2 + tex1.z * max(0, dot(L, world_normal)) + material_ks*pow(max(0, dot(H, world_normal)), material_shininess);

	if(has_alpha>0.5) if((tex2.r<0.1) && (tex2.g<0.1) && (tex2.b<0.1)) discard;

	out_color = vec4(tex1, 1);
}