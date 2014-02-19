#version 330

layout(location = 0) in vec3 in_position1;
layout(location = 1) in vec3 in_normal1;		
layout(location = 2) in vec2 in_texcoord1;
layout(location = 3) in vec3 in_position2;
layout(location = 4) in vec3 in_normal2;		
layout(location = 5) in vec2 in_texcoord2;
layout(location = 6) in vec3 in_position3;
layout(location = 7) in vec3 in_normal3;		
layout(location = 8) in vec2 in_texcoord3;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float time;

out vec2 texcoord;
out vec3 world_pos;
out vec3 world_normal;

void main(){

	texcoord = in_texcoord1;
	vec3 in_position, in_normal;

	if(time < 1500) {
		in_position = in_position1 * (1 - time/1500) + in_position2 * (time / 1500);
		in_normal = in_normal1 * (1 - time/1500) + in_normal2 * (time / 1500);
	}
	else if(time < 3000) {
		in_position = in_position2 * (1 - (time-1500)/1500) + in_position3 * ((time-1500) / 1500);
		in_normal = in_normal2 * (1 - (time-1500)/1500) + in_normal3 * ((time-1500) / 1500);
	}
	else if(time < 4500) {
		in_position = in_position3 * (1 - (time-3000)/1500) + in_position1 * ((time-3000) / 1500);
		in_normal = in_normal3 * (1 - (time-3000)/1500) + in_normal1 * ((time-3000) / 1500);
	}
		
	world_pos = (model_matrix * vec4(in_position,1)).xyz;
	world_normal = normalize(mat3(model_matrix) *  in_normal);

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
