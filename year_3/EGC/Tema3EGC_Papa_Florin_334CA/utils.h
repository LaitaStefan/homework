#include <math.h>
#include "lab_mesh_loader.hpp"

#define PI 3.14159265358979323846

//desenare teren
void DrawField(int width, 
			   int height, 
			   int laneWidth, 
			   unsigned int &vao, 
			   unsigned int& vbo, 
			   unsigned int &ibo, 
			   unsigned int &num_indices,
			   glm::vec3 colors) {

	std::vector<lab::VertexFormat> vertices;
	std::vector<unsigned int> indices;

	std::cout<<"Mesh Loader : am incarcat terenul "<<std::endl;
	int count = 0;

	for(int i = 0; i < 4; i ++) { 

		int w = i*laneWidth + width / 2;
		vertices.push_back(lab::VertexFormat(w, 0, height / 2, colors.x, colors.y, colors.z));
		indices.push_back(count);
		count++;

		vertices.push_back(lab::VertexFormat(w, 0, -height / 2, colors.x, colors.y, colors.z));
		indices.push_back(count);
		count++;

		for(int j = 29; j >= 0; j--) {
			vertices.push_back(lab::VertexFormat(cos(PI + j * PI / 30) * w, 0, -height / 2 + sin(PI + j * PI / 30) * w, colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
		}

		vertices.push_back(lab::VertexFormat(-w, 0, -height / 2, colors.x, colors.y, colors.z));
		indices.push_back(count);
		count++;

		vertices.push_back(lab::VertexFormat(-w, 0, height / 2, colors.x, colors.y, colors.z));
		indices.push_back(count);
		count++;

		for(int j = 29; j >= 0; j--) {
			vertices.push_back(lab::VertexFormat(cos(j * PI / 30) * w, 0, height / 2 + sin(j * PI / 30) * w, colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
		}
	}

	//creeaza obiectele OpenGL necesare desenarii
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
	num_indices = indices.size() / 4;
}

//desenare linie intre 2 puncte
void drawLine(int x1, int y1, int z1, 
			  int x2, int y2, int z2, 
				unsigned int &vao, 
				unsigned int& vbo, 
				unsigned int &ibo, 
				unsigned int &num_indices,
				glm::vec3 colors) {

	std::vector<lab::VertexFormat> vertices;
	std::vector<unsigned int> indices;

	vertices.push_back(lab::VertexFormat(x1, y1, z1, colors.x, colors.y, colors.z));
	indices.push_back(0);
			
	vertices.push_back(lab::VertexFormat(x2, y2, z2, colors.x, colors.y, colors.z));
	indices.push_back(1);

	//creeaza obiectele OpenGL necesare desenarii
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

//desenare steag
void drawFlag(unsigned int &vao, 
				unsigned int& vbo, 
				unsigned int &ibo, 
				unsigned int &num_indices,
				glm::vec3 colors,
				int offset) {

	std::vector<lab::VertexFormat> vertices;
	std::vector<unsigned int> indices;
	int count = 0;

	for(int i = 20; i <= 44; i += 6) {
		for(int j = 0; j <= 41; j ++) {
			vertices.push_back(lab::VertexFormat(j,i,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
			vertices.push_back(lab::VertexFormat(j,i + 6,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
			vertices.push_back(lab::VertexFormat(j + 1,i + 6,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;

			vertices.push_back(lab::VertexFormat(j,i,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
			vertices.push_back(lab::VertexFormat(j + 1,i + 6,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;
			vertices.push_back(lab::VertexFormat(j + 1,i,offset,colors.x, colors.y, colors.z));
			indices.push_back(count);
			count++;			
		}
	}

	//creeaza obiectele OpenGL necesare desenarii
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