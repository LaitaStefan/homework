//Papa Florin, 334CA
#pragma once
#include <vector>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"
#include "Wheel.h"
#include "Framework\Visual2D.h"
#include "Framework\DrawingWindow.h"

#define PI 3.14159265358979323846

class Car {
public:
	Wheel *w1, *w2, *w3, *w4;
	Object3D *body;
	Object3D *windows;
	int width, height;
	float myRot;

public:
	Car(int w, int h) {
		width = w;
		height = h;
		myRot = 0;
		create_body();
		create_windows();
		put_wheels();
	}

	Car(int w, int h, Color c1, Color c2) {
		width = w;
		height = h;
		myRot = 0;
		create_body();
		create_windows();
		windows->color.r = c2.r;	windows->color.g = c2.g;	windows->color.b = c2.b;
		windows->fill = true;
		body->color.r = c1.r;	body->color.g = c1.g;	body->color.b = c1.b;
		body->fill = true;
		put_wheels();
	}

	Car(int w, int h, Color c1, Color c2, bool b) {
		width = w;
		height = h;
		myRot = 0;
		create_body();
		create_windows();
		windows->color.r = c2.r;	windows->color.g = c2.g;	windows->color.b = c2.b;
		windows->fill = !b;
		body->color.r = c1.r;	body->color.g = c1.g;	body->color.b = c1.b;
		body->fill = b;
		put_wheels();
	}

	//creez partea de jos a masinii
	void create_body() {
		vector<Point3D*> vertices;
		vector<Face*> faces;
		vector<int> contour;

		vertices.push_back(new Point3D(-width/2, height/10, -height/2));
		vertices.push_back(new Point3D(width/2, height/10, -height/2));
		vertices.push_back(new Point3D(width/2, height/10, height/2));
		vertices.push_back(new Point3D(-width/2, height/10, height/2));
		vertices.push_back(new Point3D(-width/2, 8*height/30, -height/2));
		vertices.push_back(new Point3D(width/2, 8*height/30, -height/2));
		vertices.push_back(new Point3D(width/2, 8*height/30, height/2));
		vertices.push_back(new Point3D(-width/2, 8*height/30, height/2));
		
		//fata jos
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		//fata sus
		contour.clear();
		contour.push_back(4);
		contour.push_back(5);
		contour.push_back(6);
		contour.push_back(7);
		faces.push_back(new Face(contour));

		//fata fata
		contour.clear();
		contour.push_back(2);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(6);
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
		contour.push_back(1);
		contour.push_back(0);
		contour.push_back(4);
		contour.push_back(5);
		faces.push_back(new Face(contour));

		//fata stanga
		contour.clear();
		contour.push_back(0);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		body = new Object3D(vertices, faces);
	}

	void create_windows() {
		vector<Point3D*> vertices;
		vector<Face*> faces;
		vector<int> contour;

		vertices.push_back(new Point3D(-width/2, 8*height/30, -height/4));
		vertices.push_back(new Point3D(width/2, 8*height/30, -height/4));
		vertices.push_back(new Point3D(width/2, 8*height/30, height/4));
		vertices.push_back(new Point3D(-width/2, 8*height/30, height/4));
		vertices.push_back(new Point3D(-width/2, 13*height/30, -3*height/16));
		vertices.push_back(new Point3D(width/2, 13*height/30, -3*height/16));
		vertices.push_back(new Point3D(width/2, 13*height/30, 3*height/16));
		vertices.push_back(new Point3D(-width/2, 13*height/30, 3*height/16));

		//fata jos
		contour.push_back(0);
		contour.push_back(1);
		contour.push_back(2);
		contour.push_back(3);
		faces.push_back(new Face(contour));

		//fata sus
		contour.clear();
		contour.push_back(4);
		contour.push_back(5);
		contour.push_back(6);
		contour.push_back(7);
		faces.push_back(new Face(contour));

		//fata fata
		contour.clear();
		contour.push_back(2);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(6);
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
		contour.push_back(1);
		contour.push_back(0);
		contour.push_back(4);
		contour.push_back(5);
		faces.push_back(new Face(contour));

		//fata stanga
		contour.clear();
		contour.push_back(0);
		contour.push_back(3);
		contour.push_back(7);
		contour.push_back(4);
		faces.push_back(new Face(contour));

		windows = new Object3D(vertices, faces);
	}

	void put_wheels() {
		w1 = new Wheel(new Point3D(width/2, height/10, height / 3), height / 10);
		w2 = new Wheel(new Point3D(width/2, height/10, -height / 3), height / 10);
		w3 = new Wheel(new Point3D(-width/2 - 3*height/40, height/10, -height / 3), height / 10);
		w4 = new Wheel(new Point3D(-width/2 - 3*height/40, height/10, height / 3), height / 10);
	}

	void add_objects(Visual2D *v) {
		DrawingWindow::addObject3D_to_Visual2D(windows, v);
		DrawingWindow::addObject3D_to_Visual2D(body, v);
		DrawingWindow::addObject3D_to_Visual2D(w1, v);
		DrawingWindow::addObject3D_to_Visual2D(w2, v);
		DrawingWindow::addObject3D_to_Visual2D(w3, v);
		DrawingWindow::addObject3D_to_Visual2D(w4, v);
	}

	void remove_objects(Visual2D *v) {
		DrawingWindow::removeObject3D_from_Visual2D(body, v);
		DrawingWindow::removeObject3D_from_Visual2D(windows, v);
		DrawingWindow::removeObject3D_from_Visual2D(w1, v);
		DrawingWindow::removeObject3D_from_Visual2D(w2, v);
		DrawingWindow::removeObject3D_from_Visual2D(w3, v);
		DrawingWindow::removeObject3D_from_Visual2D(w4, v);
	}

	void move_car(float shift, float turn, float rot) {
		myRot += rot/100;
		Transform3D::loadIdentityModelMatrix();
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(0,250,760);
		Transform3D::rotateMatrixOy(turn);
		Transform3D::translateMatrix(shift, 0, 430);
		Transform3D::applyTransform(body);
		Transform3D::applyTransform(windows);

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(0, -height/10, -height/3);
		Transform3D::rotateMatrixOx(myRot);
		Transform3D::translateMatrix(0, height/10, height/3);
		Transform3D::rotateMatrixOy(turn);
		Transform3D::translateMatrix(shift, 0, 430);
		Transform3D::applyTransform(w1);

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(0, -height/10, height/3);
		Transform3D::rotateMatrixOx(myRot);
		Transform3D::translateMatrix(0, height/10, -height/3);
		Transform3D::rotateMatrixOy(turn);
		Transform3D::translateMatrix(shift, 0, 430);
		Transform3D::applyTransform(w2);

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(0, -height/10, height/3);
		Transform3D::rotateMatrixOx(myRot);
		Transform3D::translateMatrix(0, height/10, -height/3);
		Transform3D::rotateMatrixOy(turn);
		Transform3D::translateMatrix(shift, 0, 430);
		Transform3D::applyTransform(w3);

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(0, -height/10, -height/3);
		Transform3D::rotateMatrixOx(myRot);
		Transform3D::translateMatrix(0, height/10, height/3);
		Transform3D::rotateMatrixOy(turn);
		Transform3D::translateMatrix(shift, 0, 430);
		Transform3D::applyTransform(w4);		
	}
};
