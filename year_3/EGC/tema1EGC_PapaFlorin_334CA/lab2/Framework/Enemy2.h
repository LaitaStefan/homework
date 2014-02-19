//Papa Florin, 334CA

#include <iostream>
#include "Circle2D.h"

class Enemy2 {
public:
	Circle2D *c1, *c2;
	float offset_x, offset_y;
	int hits;

	Enemy2() {
		hits = 0;
	}

	//adaugare la contextul vizual
	void addSecondEnemy(Visual2D *game, int width, int height) {

		c1 = new Circle2D(Point2D(-12.5, 0), 25, Color(1,0,0), false);
		c2 = new Circle2D(Point2D(12.5, 0), 25, Color(1,0,0), false);
		offset_x = width / 2;
		offset_y = 5*height / 6 + 60;

		DrawingWindow::addObject2D_to_Visual2D(c1, game);
		DrawingWindow::addObject2D_to_Visual2D(c2, game);

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(offset_x, offset_y);
		Transform2D::applyTransform(c1);
		Transform2D::applyTransform(c2);
	}

	//mutare in pozitia initiala
	void move_second_enemy(int width, int height) {
		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(width / 2, 5*height / 6 + 80);
		Transform2D::applyTransform(c1);
		Transform2D::applyTransform(c2);
		offset_x = width / 2;
		offset_y = 5*height / 6 + 60;
	}

	//eliminare din contextul vizual
	void remove(Visual2D *game) {

		DrawingWindow::removeObject2D_from_Visual2D(c1, game);
		DrawingWindow::removeObject2D_from_Visual2D(c2, game);
	}
};