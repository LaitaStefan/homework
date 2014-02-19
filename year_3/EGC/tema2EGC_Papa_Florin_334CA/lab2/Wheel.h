//Papa Florin, 334CA
#pragma once
#include <vector>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

#define PI 3.14159265358979323846

class Wheel : public Object3D {
public:
	Wheel(Point3D *p, int r) {
		vector <int> contour;

		//adaug punctele rotilor
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(p->x, r * sin(i*2*PI/20) + p->y, r * cos(i*2*PI/20) + p->z));
			transf_vertices.push_back(new Point3D(p->x, r * sin(i*2*PI/20) + p->y, r * cos(i*2*PI/20)+ p->z));
			contour.push_back(i);
		}
		faces.push_back(new Face(contour));

		contour.clear();
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(r + p->x, r * sin(i*2*PI/20) + p->y, r * cos(i*2*PI/20) + p->z));
			transf_vertices.push_back(new Point3D(r + p->x, r * sin(i*2*PI/20) + p->y, r * cos(i*2*PI/20) + p->z));
			contour.push_back(i + 20);
		}
		faces.push_back(new Face(contour));

		for(int i = 0; i < 20; i++) {
			contour.clear();
			contour.push_back(i);
			contour.push_back((i+10) % 20);
			contour.push_back(20 + (i+10) % 20);
			contour.push_back(i + 20 );
			faces.push_back(new Face(contour));
		}
	}
};