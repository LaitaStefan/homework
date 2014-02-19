//Papa Florin, 334CA

#include "lab_mesh_loader.hpp"

//creeaza un obiect de tip nota muzicala
void drawNote(unsigned int &vao, unsigned int& vbo, unsigned int &ibo, unsigned int &num_indices,
			   glm::vec3 colors) { 

	std::vector<lab::VertexFormat> vertices;
	std::vector<unsigned int> indices;

	vertices.push_back(lab::VertexFormat(10, 10, 0, colors.x, colors.y, colors.z, 0, 1));
	vertices.push_back(lab::VertexFormat(10, -10, 0, colors.x, colors.y, colors.z, 0, 0));
	vertices.push_back(lab::VertexFormat(-10, -10, 0, colors.x, colors.y, colors.z, 1, 0));
	vertices.push_back(lab::VertexFormat(-10, 10, 0, colors.x, colors.y, colors.z, 1, 1));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);


	unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

	//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	//vertex buffer object -> un obiect in care tinem vertecsii
	glGenBuffers(1,&gl_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(lab::VertexFormat), &vertices[0], GL_STATIC_DRAW);

	//index buffer object -> un obiect in care tinem indecsii
	glGenBuffers(1,&gl_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);					//trimite pozitii pe pipe 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));	//trimite normale pe pipe 1
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

	vao = gl_vertex_array_object;
	vbo = gl_vertex_buffer_object;
	ibo = gl_index_buffer_object;
	num_indices = indices.size();
}
