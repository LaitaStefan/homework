#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_texcoords;

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec3 color;
out vec2 texcoords;

void main(){
	vec3 new_position = vec3(0,0,0);

	color = in_normal;
	texcoords = in_texcoords;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(new_position,1) + vec4(in_position, 0);
}
