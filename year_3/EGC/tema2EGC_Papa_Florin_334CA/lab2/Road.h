//Papa Florin, 334CA
#pragma once
#include <vector>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"
#include "Framework\Visual2D.h"
#include "Framework\DrawingWindow.h"

#define PI 3.14159265358979323846

class Road : public Object3D {
public:
	int width, height;

public:
	Road() {
		width = 360;
		height = 710;
		vector<int> contour;
		color.r = 0.5;
		color.g = 0.5;
		color.b = 0.5;

		vertices.push_back(new Point3D(-width/2, 0, -15000));
		vertices.push_back(new Point3D(width/2, 0, -15000));
		vertices.push_back(new Point3D(width/2, 0, height));
		vertices.push_back(new Point3D(-width/2, 0, height));
		transf_vertices.push_back(new Point3D(-width/2, 0, -15000));
		transf_vertices.push_back(new Point3D(width/2, 0, -15000));
		transf_vertices.push_back(new Point3D(width/2, 0, height));
		transf_vertices.push_back(new Point3D(-width/2, 0, height));

		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

	}

	void view() {
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(0,250,760);
		Transform3D::applyTransform(this);	
	}

};