//Papa Florin, 334CA
#pragma once
#include <vector>
#include <math.h>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

class Cube : public Object3D {
public:
	int r;
public:
	Cube(int rad, Color c) {
		color.r = c.r;
		color. g = c.g;
		color.b = c.b;
		fill = true;
		r = rad;
		vector<int> contour;

		vertices.push_back(new Point3D(-r/2, 0, -r));
		vertices.push_back(new Point3D(r/2, 0, -r));
		vertices.push_back(new Point3D(r/2, 0, r));
		vertices.push_back(new Point3D(-r/2, 0, r));
		transf_vertices.push_back(new Point3D(-r/2, 0, -r));
		transf_vertices.push_back(new Point3D(r/2, 0, -r));
		transf_vertices.push_back(new Point3D(r/2, 0, r));
		transf_vertices.push_back(new Point3D(-r/2, 0, r));

		vertices.push_back(new Point3D(-r/2, r, -r));
		vertices.push_back(new Point3D(r/2, r, -r));
		vertices.push_back(new Point3D(r/2, r, r));
		vertices.push_back(new Point3D(-r/2, r, r));
		transf_vertices.push_back(new Point3D(-r/2, r, -r));
		transf_vertices.push_back(new Point3D(r/2, r, -r));
		transf_vertices.push_back(new Point3D(r/2, r, r));
		transf_vertices.push_back(new Point3D(-r/2, r, r));

		//fata jos
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		//fata fata
		contour.clear();
		contour.push_back(7);
		contour.push_back(6);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		//fata dreapta
		contour.clear();
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(6);
		contour.push_back(5);
		faces.push_back(new Face(contour));

		//fata spate
		contour.clear();
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(5);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		//fata stnga
		contour.clear();
		contour.push_back(0);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		//fata sus
		contour.clear();
		contour.push_back(4);
		contour.push_back(5);
		contour.push_back(6);
		contour.push_back(7);
		faces.push_back(new Face(contour));
	}
};