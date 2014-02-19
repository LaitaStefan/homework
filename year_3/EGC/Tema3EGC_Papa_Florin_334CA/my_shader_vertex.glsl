//Papa Florin, 334CA

#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_color;		

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float time, mode;

out vec3 vertex_to_fragment_color;

void main(){

	if(mode == 0) vertex_to_fragment_color = in_color;
	else vertex_to_fragment_color = vec3(0,0,0);

	if(in_position.x != 0 && in_position.x / 7 >= 0.1) in_position.z = sin(in_position.x*20 + time)*3;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
