//Papa Florin, 334CA
#pragma once
#include <vector>
#include <math.h>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

class Tethra : public Object3D {
public:
	int r;
public:
	Tethra(int rad, Color c) {
		color.r = c.r;
		color.g = c.g;
		color.b = c.b;
		r = rad;
		vector<int> contour;

		vertices.push_back(new Point3D(-r/2, 0, -r/2));
		vertices.push_back(new Point3D(r/2, 0, -r/2));
		vertices.push_back(new Point3D(r/2, 0, r/2));
		vertices.push_back(new Point3D(-r/2, 0, r/2));
		vertices.push_back(new Point3D(0, r, 0));
		transf_vertices.push_back(new Point3D(-r/2, 0, -r/2));
		transf_vertices.push_back(new Point3D(r/2, 0, -r/2));
		transf_vertices.push_back(new Point3D(r/2, 0, r/2));
		transf_vertices.push_back(new Point3D(-r/2, 0, r/2));
		transf_vertices.push_back(new Point3D(0, r, 0));

		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(2);
		contour.push_back(3);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		contour.clear();
		contour.push_back(3);
		contour.push_back(0);
		contour.push_back(4);
		faces.push_back(new Face(contour));
	}
};
		