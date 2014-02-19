//Papa Florin, 334CA
#pragma once
#include <vector>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"
#include "Framework\Visual2D.h"
#include "Framework\DrawingWindow.h"
#include "Framework\Transform3D.h"

#define PI 3.14159265358979323846

class Stripes {
public:
	int width, height;
	vector<Object3D*> stripe;
	float offset;
public:
	Stripes() {
		width = 15;
		height = 200;
		offset = 0;
		int aux = 0;

		for(int i = 0; i < 25; i++) {
			vector<int> contour;
			vector<Point3D*> vertices;
			vector<Face*> faces;

			vertices.push_back(new Point3D(-width/2 + 60, 0, -height/2 + aux - i*2*height + height));
			vertices.push_back(new Point3D(width/2 + 60, 0, -height/2 + aux - i*2*height+height));
			vertices.push_back(new Point3D(width/2 + 60, 0, height/2 + aux - i*2*height+height));
			vertices.push_back(new Point3D(-width/2 + 60, 0, height/2 + aux - i*2*height+height));

			contour.push_back(0);
			contour.push_back(1);
			contour.push_back(2);
			contour.push_back(3);

			faces.push_back(new Face(contour));
			stripe.push_back(new Object3D(vertices, faces, Color(1,1,1), true));
		}

		for(int i = 0; i < 25; i++) {
			vector<int> contour;
			vector<Point3D*> vertices;
			vector<Face*> faces;

			vertices.push_back(new Point3D(-width/2 - 60, 0, -height/2 + aux - i*2*height + height));
			vertices.push_back(new Point3D(width/2 - 60, 0, -height/2 + aux - i*2*height+height));
			vertices.push_back(new Point3D(width/2 - 60, 0, height/2 + aux - i*2*height+height));
			vertices.push_back(new Point3D(-width/2 - 60, 0, height/2 + aux - i*2*height+height));

			contour.push_back(0);
			contour.push_back(1);
			contour.push_back(2);
			contour.push_back(3);

			faces.push_back(new Face(contour));
			stripe.push_back(new Object3D(vertices, faces, Color(1,1,1), true));
		}
	}

	void addAll_to_visual2D(Visual2D *v) {
		for(int i = 0; i < 50; i++) {
			DrawingWindow::addObject3D_to_Visual2D(stripe[i], v);
		}
	}

	void move(float speed) {
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::translateMatrix(0,0,offset);
		Transform3D::perspectiveProjectionMatrix(0,250,760);

		for(int i = 0; i < 50; i++) {
			Transform3D::applyTransform(stripe[i]);
		}
		offset += speed;
		if(offset >= 400) {
			offset = 0;
		}
	}
};