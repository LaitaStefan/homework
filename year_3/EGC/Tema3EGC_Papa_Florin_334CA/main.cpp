//Papa Florin, 334CA

//incarcator de meshe
#include "lab_mesh_loader.hpp"

//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

//incarcator de shadere
#include "lab_shader_loader.hpp"

//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

//camera
#include "lab_camera.hpp"

//time
#include <ctime>
#include <math.h>
#include <fstream>
#include "utils.h"
#include "CarMovement.h"
using namespace std;

class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix, projection_matrix, aux_p, aux_p1, aux_p2, aux_p3;			//matrici 4x4 pt modelare vizualizare proiectie
	lab::Camera cam[5];																	//obiectele camera
	int finish, crossed;																//indica terminarea jocului
	float time, prevZ, mode;			
	unsigned int gl_program_shader, gl_my_shader;										//id-ul de opengl al obiectului de tip program shader
	unsigned int screen_width, screen_height;
	unsigned int mesh_vbo[10], mesh_ibo[10], mesh_vao[10], mesh_num_indices[10];		//containere opengl pentru vertecsi, indecsi si stare
	float moveF, moveB, moveR, moveL;													//taste apasate
	CarMovement *mov;
	AdvMovement *adv1, *adv2;
	glm::vec3 pl_color, adv1_color, adv2_color, finish_color;
	int currentCam;																		//camera curenta

//metode	
public:
	
	//constructor
	Laborator(){
		moveF = moveR = moveB = moveL = finish = time = 0;
		prevZ = -1;
		crossed = 0;
		mov = new CarMovement(160, 400);

		//citire viteze adversari din fisier
		std::ifstream f("speed.txt");
		float s1, s2;
		f >> s1;
		f >> s2;
		f.close();

		adv1 = new AdvMovement(45, -30, 160, 400, s1);
		adv2 = new AdvMovement(75, -60, 160, 400, s2);
		currentCam = 0;

		pl_color = glm::vec3(1,0,0);
		adv1_color = glm::vec3(0,1,0);
		adv2_color = glm::vec3(0,0,1);

		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		gl_my_shader = lab::loadShader("my_shader_vertex.glsl", "my_fragment_shader.glsl");
		
		//incarca un mesh
		lab::loadObj("resurse\\camaro.obj",mesh_vao[0], mesh_vbo[0], mesh_ibo[0], mesh_num_indices[0], pl_color); //jucator
		lab::loadObj("resurse\\police_car.obj",mesh_vao[1], mesh_vbo[1], mesh_ibo[1], mesh_num_indices[1], adv1_color); //adversar 1
		lab::loadObj("resurse\\police_car.obj",mesh_vao[2], mesh_vbo[2], mesh_ibo[2], mesh_num_indices[2], adv2_color); //adversar 2
		lab::loadObj("resurse\\cameraTV.obj",mesh_vao[3], mesh_vbo[3], mesh_ibo[3], mesh_num_indices[3], glm::vec3(1,1,0)); //camera laterala
		DrawField(160,400,30,mesh_vao[4], mesh_vbo[4], mesh_ibo[4], mesh_num_indices[4], glm::vec3(0,0,1));
		drawLine(80, 0, 0, 170, 0, 0, mesh_vao[5], mesh_vbo[5], mesh_ibo[5], mesh_num_indices[5], glm::vec3(0,0,1));	

		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(10);
		glPointSize(10);

		cam[0].set(glm::vec3(0, 500, 0), glm::vec3(0, 0, 0), glm::vec3(0,0,1));
	}

	//destructor
	~Laborator(){
		//distruge shader
		glDeleteProgram(gl_program_shader);

		//distruge mesh incarcat
		for(int i = 0; i < 5; i++) {
			glDeleteBuffers(1,&mesh_vbo[i]);
			glDeleteBuffers(1,&mesh_ibo[i]);
			glDeleteVertexArrays(1,&mesh_vao[i]);
		}
	}

	//verifica daca s-a terminat jocul
	void checkFinish() {
		if(mov->x > 0 && mov->z > 0 && prevZ < 0) crossed--;
		else if(mov->x > 0 && mov->z < 0 && prevZ > 0) {
			if(crossed == 0) {
				finish = 1;
				finish_color = pl_color;
			}
			else crossed++;
		}

		if(adv1->x > 0 && floor(adv1->z) == 0) {
			finish = 1;
			finish_color = adv1_color;
		}

		if(adv2->x > 0 && floor(adv2->z) == 0) {
			finish = 1;
			finish_color = adv2_color;
		}

		if(finish == 1) {
			cam[currentCam].set(glm::vec3(14,20,58), glm::vec3(20,20,0), glm::vec3(0,1,0));
			drawLine(0.99,-50,1,0.99,50,1, mesh_vao[6], mesh_vbo[6], mesh_ibo[6], mesh_num_indices[6], glm::vec3(0,0,0));
			drawFlag(mesh_vao[7], mesh_vbo[7], mesh_ibo[7], mesh_num_indices[7], finish_color, 0);
		}
	}

	
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
		if(finish == 0) {
			//miscare jucator
			prevZ = mov->z;
			mov->updateParams(moveF, moveB, moveR, moveL);
			aux_p = mov->moveCar();
			cam[1].set(glm::vec3(mov->x, 10, mov->z), glm::vec3(mov->x, 10, mov->z - 10), glm::vec3(0,1,0));
			cam[1].rotateFPSoY(-PI * ((mov->rot - 270) / 180));

			//miscare adversar 1
			aux_p1 = adv1->move();
			cam[2].set(glm::vec3(adv1->x, 10, adv1->z), glm::vec3(adv1->x, 10, adv1->z - 10), glm::vec3(0,1,0));
			cam[2].rotateFPSoY(-PI * ((adv1->rot - 270) / 180));

			//miscare adversar 2
			aux_p2 = adv2->move();
			cam[3].set(glm::vec3(adv2->x, 10, adv2->z), glm::vec3(adv2->x, 10, adv2->z - 10), glm::vec3(0,1,0));
			cam[3].rotateFPSoY(-PI * ((adv2->rot - 270) / 180));

			//miscare camera laterala
			aux_p3 = adv2->moveCam(mov->x, mov->z);
			cam[4].set(glm::vec3(adv2->newX, 10, adv2->newZ), glm::vec3(mov->x, 10, mov->z), glm::vec3(0,1,0));
		}
	}

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		

		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLineWidth(1);
		
		if(finish == 0) {
			//miscare masina
			{
				glViewport(0,0, screen_width, screen_height);
			
				//foloseste shaderul
				glUseProgram(gl_program_shader);

				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(aux_p));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		
				//bind obiect
				glBindVertexArray(mesh_vao[0]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices[0], GL_UNSIGNED_INT, 0);
			}
			
			//adversar 1
			{
				//foloseste shaderul
				glUseProgram(gl_program_shader);

				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(aux_p1));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		
				//bind obiect
				glBindVertexArray(mesh_vao[1]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices[1], GL_UNSIGNED_INT, 0);
			}

			//adversar 2
			{
				//foloseste shaderul
				glUseProgram(gl_program_shader);

				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(aux_p2));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		
				//bind obiect
				glBindVertexArray(mesh_vao[2]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices[2], GL_UNSIGNED_INT, 0);
			}

			//camera
			{
				//foloseste shaderul
				glUseProgram(gl_program_shader);

				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(aux_p3));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		
				//bind obiect
				glBindVertexArray(mesh_vao[3]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices[3], GL_UNSIGNED_INT, 0);
			}

			//teren
			{
				glUseProgram(gl_program_shader);
				
				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			
				glLineWidth(0.6);
				for(int i = 0; i < 4; i++) {
					glBindVertexArray(mesh_vao[4]);
					glDrawElements(GL_LINE_LOOP, mesh_num_indices[4], GL_UNSIGNED_INT, (char*)NULL + (i * mesh_num_indices[4] * sizeof(unsigned int)));
				}
			}

			//linie finish
			{
				glUseProgram(gl_program_shader);
				
				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			
				glLineWidth(0.6);
				glBindVertexArray(mesh_vao[5]);
				glDrawElements(GL_LINE_LOOP, mesh_num_indices[5], GL_UNSIGNED_INT, 0);
			}

			checkFinish();	//verificare finish
		}
		else {
			//batz steag
			{
				glUseProgram(gl_program_shader);
				
				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			
				glLineWidth(8);
				glBindVertexArray(mesh_vao[6]);
				glDrawElements(GL_LINE_LOOP, mesh_num_indices[6], GL_UNSIGNED_INT, 0);

			}

			//steag
			{	
				glDisable(GL_DEPTH_TEST);
				glUseProgram(gl_my_shader);

				mode = 0;	//colorare culoare transmisa
				//trimite variabile uniforme la shader
				glUniformMatrix4fv(glGetUniformLocation(gl_my_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
				glUniformMatrix4fv(glGetUniformLocation(gl_my_shader,"view_matrix"),1,false,glm::value_ptr(cam[currentCam].getViewMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(gl_my_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
				glUniform1f(glGetUniformLocation(gl_my_shader,"time"), time);
				glUniform1f(glGetUniformLocation(gl_my_shader,"mode"), mode);
			

				glLineWidth(0.6);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(mesh_vao[7]);
				glDrawElements(GL_TRIANGLE_FAN, mesh_num_indices[7], GL_UNSIGNED_INT, 0);

				mode = 1;	//colorare negru
				glUniform1f(glGetUniformLocation(gl_my_shader,"mode"), mode);
				glLineWidth(0.2);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(mesh_vao[7]);
				glDrawElements(GL_TRIANGLES, mesh_num_indices[7], GL_UNSIGNED_INT, 0);
			}

			//variabila uniforma trimisa shader-ului pentru a realiza fluturarea
			time += 0.01;
		}		
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		screen_width = width;
		screen_height = height;
		float aspect = width*0.5f / height;
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
		if(key_pressed == 'i'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}
		if(key_pressed == 'w') { cam[0].translateForward(4.0f); }
		if(key_pressed == 'a') { cam[0].translateRight(-4.0f); }
		if(key_pressed == 's') { cam[0].translateForward(-4.0f); }
		if(key_pressed == 'd') { cam[0].translateRight(4.0f); }
		if(key_pressed == 'r') { cam[0].translateUpword(4.0f); }
		if(key_pressed == 'f') { cam[0].translateUpword(-4.0f); }
		if(key_pressed == 'q') { cam[0].rotateFPSoY(-0.1f); }
		if(key_pressed == 'e') { cam[0].rotateFPSoY(0.1f); }
		if(key_pressed == 'z') { cam[0].rotateFPSoZ(-0.1f); }
		if(key_pressed == 'c') { cam[0].rotateFPSoZ(0.1f); }
		if(key_pressed == 't') { cam[0].rotateFPSoX(0.1f); }
		if(key_pressed == 'g') { cam[0].rotateFPSoX(-0.1f); }
		if(key_pressed == 'o') { cam[0].set(glm::vec3(0, 500, 0), glm::vec3(0, 0, 0), glm::vec3(0,0,1)); }
		if(key_pressed == 'v') { 
			if(finish == 0) {
				if(currentCam == 4) currentCam = 0;
				else currentCam++;
			}
		}
	}

	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}

	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
		if(key_pressed == GLUT_KEY_UP) moveF = 1;
		if(key_pressed == GLUT_KEY_DOWN) moveB = 1;
		if(key_pressed == GLUT_KEY_RIGHT) moveR = 1;
		if(key_pressed == GLUT_KEY_LEFT) moveL = 1;
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y) {
		if(key_released == GLUT_KEY_UP) moveF = 0;
		if(key_released == GLUT_KEY_DOWN) moveB = 0;
		if(key_released == GLUT_KEY_RIGHT) moveR = 0;
		if(key_released == GLUT_KEY_LEFT) moveL = 0;
	}

	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 3 EGC"),800,600,100,100,true);
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
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\ti ... toggle wireframe"<<std::endl<<"\to ... reseteaza camera"<<std::endl;
	std::cout<<"\tw ... translatie camera in fata"<<std::endl<<"\ts ... translatie camera inapoi"<<std::endl;
	std::cout<<"\ta ... translatie camera in stanga"<<std::endl<<"\td ... translatie camera in dreapta"<<std::endl;
	std::cout<<"\tr ... translatie camera in sus"<<std::endl<<"\tf ... translatie camera jos"<<std::endl;
	std::cout<<"\tq ... rotatie camera FPS pe Oy, minus"<<std::endl<<"\te ... rotatie camera FPS pe Oy, plus"<<std::endl;
	std::cout<<"\tz ... rotatie camera FPS pe Oz, minus"<<std::endl<<"\tc ... rotatie camera FPS pe Oz, plus"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}