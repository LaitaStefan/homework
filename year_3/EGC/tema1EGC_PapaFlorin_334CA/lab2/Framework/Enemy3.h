//Papa Florin, 334CA

#include <iostream>
#include "Circle2D.h"
#include "Line2D.h"

class Enemy3 {
public:
	Circle2D *face, *eye1, *eye2;
	Line2D *line;
	float offset_x, offset_y;
	int hits;

	Enemy3() {
		hits = 0;
	}

	//adaugare la contextul vizual
	void addThirdEnemy(Visual2D *game, int width, int height) {

		face = new Circle2D(Point2D(0,0), 25, Color(1, 1, 0), false);
		eye1 = new Circle2D(Point2D(12.5, 12.5), 2.5, Color(1, 1, 0), true);
		eye2 = new Circle2D(Point2D(-12.5, 12.5), 2.5, Color(1, 1, 0), true);
		line = new Line2D(Point2D(-12.5, -12.5), Point2D(12.5, -12.5), Color(1, 1, 0));
		offset_x = width + 60;
		offset_y = 5*height / 12;

		DrawingWindow::addObject2D_to_Visual2D(face, game);
		DrawingWindow::addObject2D_to_Visual2D(eye1, game);
		DrawingWindow::addObject2D_to_Visual2D(eye2, game);
		DrawingWindow::addObject2D_to_Visual2D(line, game);

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(offset_x, offset_y);
		Transform2D::applyTransform(face);
		Transform2D::applyTransform(eye1);
		Transform2D::applyTransform(eye2);
		Transform2D::applyTransform(line);
	}

	//mutare in pozitia initiala
	void move_third_enemy(int width, int height) {

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(width + 80, 5*height / 12);
		Transform2D::applyTransform(face);
		Transform2D::applyTransform(eye1);
		Transform2D::applyTransform(eye2);
		Transform2D::applyTransform(line);
		offset_x = width + 60;
		offset_y = 5*height / 12;
	}

	//eliminare din contextul vizual
	void remove(Visual2D *game) {

		DrawingWindow::removeObject2D_from_Visual2D(face, game);
		DrawingWindow::removeObject2D_from_Visual2D(eye1, game);
		DrawingWindow::removeObject2D_from_Visual2D(eye2, game);
		DrawingWindow::removeObject2D_from_Visual2D(line, game);
	}
};