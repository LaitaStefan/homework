//Papa Florin, 334CA
#pragma once
#include <vector>
#include <math.h>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

#define PI 3.14159265358979323846

class WireSphere : public Object3D {
public:
	WireSphere(int r, Color c) {
		vector<int> contour;
		int offset = 0;
		color.r = c.r;
		color.g = c.g;
		color.b = c.b;

		//adaug "feliile" orizontale
		for(int i = 0; i < 5; i++) {
			float dist = i * r / 5;
			float radius = sqrt(pow(r, 2) - pow(dist, 2));
			contour.clear();

			for(int k = 0; k < 20; k++) {
				vertices.push_back(new Point3D(radius * sin(k*2*PI/20), dist, radius * cos(k*2*PI/20)));
				transf_vertices.push_back(new Point3D(radius * sin(k*2*PI/20), dist, radius * cos(k*2*PI/20)));
				contour.push_back(offset + k);
			}

			offset += 20;
			faces.push_back(new Face(contour));

			contour.clear();
			for(int k = 0; k < 20; k++) {
				vertices.push_back(new Point3D(radius * sin(k*2*PI/20), -dist, radius * cos(k*2*PI/20)));
				transf_vertices.push_back(new Point3D(radius * sin(k*2*PI/20), -dist, radius * cos(k*2*PI/20)));
				contour.push_back(offset + k);
			}

			offset += 20;
			faces.push_back(new Face(contour));
		}


		//adaug meridianele
		for(int i = 0; i < 10; i++) {
			contour.clear();
			for(int k = 0; k < 20; k++) {
				vertices.push_back(new Point3D(-r*sin(k*2*PI/20)*sin(i*PI/10), r*cos(k*2*PI/20), r*sin(2*k*PI/20)*cos(i*PI/10)));
				transf_vertices.push_back(new Point3D(-r*sin(k*2*PI/20)*sin(i*PI/10), r*cos(k*2*PI/20), r*sin(k*2*PI/20)*cos(i*PI/10)));
				contour.push_back(offset + k);
			}

			offset += 20;
			faces.push_back(new Face(contour));
		}
	}
};