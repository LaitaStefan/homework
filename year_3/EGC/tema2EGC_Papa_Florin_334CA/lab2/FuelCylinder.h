//Papa Florin, 334CA
#pragma once
#include <vector>
#include <math.h>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

#define PI 3.14159265358979323846

class FuelCylinder {
public:
	int r, h;
	Object3D *lower, *upper;
public:
	FuelCylinder(int rad, int height) {
		r = rad;
		h = height;

		vector<int> contour;
		vector<Point3D*> vertices;
		vector<Face*> faces;

		//creez partea de jos
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(r * sin(i*2*PI/20), 0, r * cos(i*2*PI/20)));
			contour.push_back(i);
		}
		faces.push_back(new Face(contour));

		contour.clear();
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(r * sin(i*2*PI/20), 2*h/3, r * cos(i*2*PI/20)));
			contour.push_back(i + 20);
		}
		faces.push_back(new Face(contour));

		for(int i = 0; i < 20; i++) {
			contour.clear();
			contour.push_back(i);
			contour.push_back((i + 1) % 20);
			contour.push_back((i + 1) % 20 + 20);
			contour.push_back(i + 20);
			faces.push_back(new Face(contour));
		}
		lower = new Object3D(vertices, faces, Color(1,0,0), true);

		//creez partea de sus
		vertices.clear();
		contour.clear();
		faces.clear();
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(r * sin(i*2*PI/20), 2*h/3, r * cos(i*2*PI/20)));
			contour.push_back(i);
		}
		faces.push_back(new Face(contour));

		contour.clear();
		for(int i = 0; i < 20; i++) {
			vertices.push_back(new Point3D(r * sin(i*2*PI/20), h, r * cos(i*2*PI/20)));
			contour.push_back(i + 20);
		}
		faces.push_back(new Face(contour));

		for(int i = 0; i < 20; i++) {
			contour.clear();
			contour.push_back(i);
			contour.push_back((i + 1) % 20);
			contour.push_back((i + 1) % 20 + 20);
			contour.push_back(i + 20);
			faces.push_back(new Face(contour));
		}
		upper = new Object3D(vertices, faces, Color(1,0,0), false);
	}

	void remove_objects(Visual2D *v) {
		DrawingWindow::removeObject3D_from_Visual2D(lower, v);
		DrawingWindow::removeObject3D_from_Visual2D(upper, v);
	}

	void add_objects(Visual2D *v) {
		DrawingWindow::addObject3D_to_Visual2D(lower, v);
		DrawingWindow::addObject3D_to_Visual2D(upper, v);
	}
};
