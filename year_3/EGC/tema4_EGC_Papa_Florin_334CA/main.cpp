//Papa Florin, 334CA

//incarcator de meshe
#include "lab_mesh_loader.hpp"
//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"
//texturi
#include "lab_texture_loader.hpp"
#include "utils.h"
//time
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix_girl, model_matrix_note[200];				//matrici modelare pentru fata, respectiv particule 
	glm::mat4 view_matrix_note, view_matrix_girl, projection_matrix;	//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader, gl_note_shader;						//id-ul de opengl al obiectului de tip program shader
	unsigned int screen_width, screen_height;
	float time_f;					//folosit pentru a face tranzitia intre stari
	float off_x[200], off_y[200];	//offset fata de origine pentru particule

	//meshe
	unsigned int girl_vbo, girl_ibo, girl_vao, girl_num_indices;							
	unsigned int note_vbo[200], note_ibo[200], note_vao[200], note_num_indices[200];
	
	//texturi
	unsigned int girl_texture, note_texture;

	//variabile pentru iluminare
	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;
	float material_ks;

	clock_t start, end;
	int done;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		gl_note_shader = lab::loadShader("shadere\\shader_billboard_vertex.glsl", "shadere\\shader_billboard_fragment.glsl");
		
		//girl
		lab::loadMultipleObj("resurse\\girl_sleep.obj", "resurse\\girl_surprise.obj", "resurse\\girl_annoyed.obj", 
								girl_vao, girl_vbo, girl_ibo, girl_num_indices);	
		girl_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");
		model_matrix_girl = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);

		//note
		initialize_notes();
		note_texture = lab::loadTextureBMP("resurse\\music.bmp");

		//matrici de modelare si vizualizare
		view_matrix_note = glm::lookAt(glm::vec3(0,0,100), glm::vec3(0,0,0), glm::vec3(0,1,0));
		view_matrix_girl = glm::lookAt(glm::vec3(0,7,3), glm::vec3(0,7,0), glm::vec3(0,1,0));

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//lumina
		eye_position = glm::vec3(0,7,3);
		light_position = glm::vec3(0,7,10);
		material_shininess = 150;
		material_ks = 0.2;

		//contor tranzitie
		time_f = 0;
		start = clock();
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shader
		glDeleteProgram(gl_program_shader);

		//distruge obiecte
		glDeleteBuffers(1,&girl_vbo);	
		glDeleteBuffers(1,&girl_ibo);	
		glDeleteVertexArrays(1,&girl_vao);		
		glDeleteTextures(1, &girl_texture);

		for(int i = 0 ; i < 200; i++) {
			glDeleteBuffers(1,&note_vbo[i]);	
			glDeleteBuffers(1,&note_ibo[i]);	
			glDeleteVertexArrays(1,&note_vao[i]);	
		}
	}

	//initializeaza particulele si le atribuie o pozitie random
	void initialize_notes() {
		srand(time(NULL));
		for(int i = 0; i < 200; i++) {
			off_x[i] = 40 + rand() % 200;
			off_y[i] = 60 + rand() % 100;

			if(i % 3 == 0) {
				drawNote(note_vao[i], note_vbo[i], note_ibo[i], note_num_indices[i], glm::vec3(1, 0, 0));
			}
			else if(i % 3 == 1) {
				drawNote(note_vao[i], note_vbo[i], note_ibo[i], note_num_indices[i], glm::vec3(0, 1, 0));
			}
			else {
				drawNote(note_vao[i], note_vbo[i], note_ibo[i], note_num_indices[i], glm::vec3(0, 0, 1));
			}

			model_matrix_note[i] = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);

			model_matrix_note[i] = glm::translate(model_matrix_note[i], glm::vec3(-off_x[i], off_y[i], -10));
		}
	}

	void update_time() {
		while(time_f > 4500)  time_f -= 4500;
	}
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
		//modificare pas
		end = clock();
		time_f = ((double(end - start)) / CLOCKS_PER_SEC) * 250;

		update_time();

		if(time_f > 3000 && done == 0) {
			printf("se intra aici\n");
			for(int i = 0 ; i < 200; i++) {
				glDeleteBuffers(1,&note_vbo[i]);	
				glDeleteBuffers(1,&note_ibo[i]);	
				glDeleteVertexArrays(1,&note_vao[i]);	
			}
			initialize_notes();
			done = 1;
		}
		else if(time_f <= 3000 && done != 0) done = 0;
	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		
		//desenare fata
		{
			//pe tot ecranul
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			//foloseste shaderul
			glUseProgram(gl_program_shader);
				
			//trimite variabile uniforme la shader
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(view_matrix_girl));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));

			glUniform3f(glGetUniformLocation(gl_program_shader, "light_position"),light_position.x, light_position.y, light_position.z);
			glUniform3f(glGetUniformLocation(gl_program_shader, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
			glUniform1i(glGetUniformLocation(gl_program_shader, "material_shininess"),material_shininess);
			glUniform1f(glGetUniformLocation(gl_program_shader, "material_ks"),material_ks);
			glUniform1f(glGetUniformLocation(gl_program_shader, "time"),time_f);

			//girl
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix_girl));
	
			glActiveTexture(GL_TEXTURE0 + girl_texture);
			glBindTexture(GL_TEXTURE_2D, girl_texture);
			glUniform1i(glGetUniformLocation(gl_program_shader, "textura1"), girl_texture);
			glUniform1i(glGetUniformLocation(gl_program_shader, "has_alpha"), 0);

			glBindVertexArray(girl_vao);
			glDrawElements(GL_TRIANGLES, girl_num_indices, GL_UNSIGNED_INT, 0);
		}

		//desenare particule
		{
			srand(time(NULL));
			if(time_f <= 3000) {
				for(int i = 0; i < 200; i++) {
					//desenare nota
					glUseProgram(gl_note_shader);

					float x, y;
					x = (float)(rand() % 10) / 400;
					y = (float)(rand() % 10) / 400;
					model_matrix_note[i] = glm::translate(model_matrix_note[i], glm::vec3(0.06 + x, -0.06 + y, 0));
				
					//trimite variabile uniforme la shader
					glUniformMatrix4fv(glGetUniformLocation(gl_note_shader,"view_matrix"),1,false,glm::value_ptr(view_matrix_note));
					glUniformMatrix4fv(glGetUniformLocation(gl_note_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
					glUniformMatrix4fv(glGetUniformLocation(gl_note_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix_note[i]));
			
					glActiveTexture(GL_TEXTURE0 + note_texture);
					glBindTexture(GL_TEXTURE_2D, note_texture);
					glUniform1i(glGetUniformLocation(gl_note_shader, "textura"), note_texture);
					glUniform1i(glGetUniformLocation(gl_note_shader, "has_alpha"), 1);

					glBindVertexArray(note_vao[i]);
					glDrawElements(GL_TRIANGLES, note_num_indices[i], GL_UNSIGNED_INT, 0);
				}
			}
		}
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		float aspect = (float)width / (float)height;
		projection_matrix = glm::perspective(75.0f, aspect,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader);
			gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ }

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("lab shadere 5 - texturi"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}