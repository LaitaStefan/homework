//Papa Florin, 334CA

#include <math.h>

#define PI 3.14159265358979323846

//clasa care se ocupa cu miscarea jucatorului principal
class CarMovement {
public:
	float speed, maxSpeed, rot;
	float x, z, width, height;
public:
	CarMovement(float w, float h) {
		speed = 0;
		maxSpeed = 0.25;
		rot = 270;
		x = w / 2 + 15;
		z = -1;
		width = w;
		height = h;
	}

	//intoarce distanta euclidiana intre doua puncte dintr-un plan
	float dist(int x1, int y1, int x2, int y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}

	//misca masina in functie de rotatia si offset-ul calculate mai jos
	glm::mat4 moveCar() {
		float newX = x - speed * cos(PI * (rot / 180));
		float newZ = z + speed * sin(PI * (rot / 180));

		if(((newX <= width/2 + 90 && newX >= width/2) || (newX >= -width/2-90 && newX <= -width/2)) &&
			(newZ <= height/2 && newZ >= -height/2)) {
				x = newX;
				z = newZ;
				
		}

		else if((dist(newX, newZ, 0, height/2) >= 80 && dist(newX, newZ, 0, height/2) <= 165 && z >= height/2 - 1) || 
				(dist(newX, newZ, 0, -height/2) >= 80 && dist(newX, newZ, 0, -height/2) <= 165) && z <= -height/2 + 1) {
			x = newX;
			z = newZ;
		}
		else speed = 0;

		glm::mat4 aux_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		aux_matrix = glm::translate(aux_matrix, glm::vec3(x, 2, z));
		aux_matrix = glm::rotate(aux_matrix, (float)(rot - 90), glm::vec3(0,1,0));
		aux_matrix = glm::translate(aux_matrix, glm::vec3(0 , 0, 3));

		return aux_matrix;
	}

	//in functie de tastele apasate se modifica viteza si rotatia
	void updateParams(float fw, float bk, float right, float left) {		
		rot -= right*0.12;
		rot += left*0.12;
		
		if(speed < maxSpeed && fw == 1) speed += 0.001;
		if(speed > -maxSpeed && bk == 1) speed -= 0.001;

		if(fw == 0 && bk == 0 && speed >= 0.00005) speed -= 0.0001;
		else if(fw == 0 && bk == 0 && speed <= -0.00005) speed += 0.0001;
		else if(fw == 0 && bk == 0) speed = 0;
	}
};

//clasa care se ocupa cu miscarea adversarilor si a camerei de pe margine
class AdvMovement {
public:
	float speed, rot;
	float x, z, radius, width, height, newX, newZ;
public:
	AdvMovement(float x1, float z1, float w, float h, float s) {
		x = w / 2 + x1;
		z = z1 ;
		rot = 270;
		speed = s;
		radius = x;
		width = w;
		height = h;
	}

	//miscarea adversarilor
	glm::mat4 move() {
		glm::mat4 aux_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		if(z <= 200 && z >= -200) {
			z += speed * sin(PI * (rot / 180));;
			aux_matrix = glm::translate(aux_matrix, glm::vec3(x, 0, z));
			aux_matrix = glm::rotate(aux_matrix, (float)(rot - 90), glm::vec3(0,1,0));
			aux_matrix = glm::scale(aux_matrix, glm::vec3(1.5,1.5,1.5));
		}
		else {
			rot += (180 * atan(speed / radius)) / PI;
			x -= speed * cos(PI * (rot / 180));
			z += speed * sin(PI * (rot / 180));

			aux_matrix = glm::translate(aux_matrix, glm::vec3(x, 0, z));
			aux_matrix = glm::rotate(aux_matrix, (float)(rot - 90), glm::vec3(0,1,0));
			aux_matrix = glm::scale(aux_matrix, glm::vec3(1.5,1.5,1.5));
		}		
		
		return aux_matrix;
	}

	float dist(int x1, int y1, int x2, int y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}

	//miscare camera in functie de coordonatele jucatorului
	glm::mat4 moveCam(float x, float z) {
		float rot;

		//partea dreapta
		if(z <= 200 && z >= -200 && x > 0) {
			newX = width / 2 + 105;
			newZ = z;
			rot = 0;
		}
		//partea stanga
		else if(z <= 200 && z >= -200 && x < 0) {
			newX = -width / 2 - 105;
			newZ = z;
			rot = 180;
		}
		//semicerc superior
		else if(z <= -200) {
			rot = (180 * atan((-200 - z) / abs(x))) / PI;
			newX = (width / 2 + 105) * cos((PI * rot) / 180);
			newZ = -200 - (width / 2 + 105) * sin((PI * rot) / 180);
			if(x < 0) {
				newX = -newX;
				rot = 180 - rot;
			}
		}
		//semicerc inferior
		else if(z >= 200) {
			rot = 180 + (180 * atan((z - 200) / abs(x))) / PI;
			newX = (width / 2 + 105) * cos((PI * rot) / 180);
			newZ = 200 - (width / 2 + 105) * sin((PI * rot) / 180);
			if(x > 0) {
				newX = -newX;
				rot = 180 - rot;
			}
		}

		glm::mat4 aux_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		aux_matrix = glm::translate(aux_matrix, glm::vec3(newX, 5, newZ));
		aux_matrix = glm::rotate(aux_matrix, (float)(rot + 90), glm::vec3(0,1,0));
		aux_matrix = glm::rotate(aux_matrix, (float)90, glm::vec3(1,0,0));
		aux_matrix = glm::scale(aux_matrix, glm::vec3(2,2,2));

		return aux_matrix;
	}
};