//Papa Florin, 334CA

#include <iostream>
#include "Rectangle2D.h"

class Enemy4 {
public:
	Rectangle2D *r1, *r2, *r3, *r4;
	float offset_x, offset_y;
	int hits;

	Enemy4() {
		hits = 0;
	}

	//adaug inamicul in contextul vizual
	void addFourthEnemy(Visual2D *game, int width) {

		r1 = new Rectangle2D(Point2D(-30, -20), 20, 20, Color(0, 1, 1), false);
		r2 = new Rectangle2D(Point2D(-10, -20), 20, 20, Color(0, 1, 1), false);
		r3 = new Rectangle2D(Point2D(10, -20), 20, 20, Color(0, 1, 1), false);
		r4 = new Rectangle2D(Point2D(-10, 0), 20, 20, Color(0, 1, 1), false);
		offset_x = width / 2;
		offset_y = -60;

		DrawingWindow::addObject2D_to_Visual2D(r1, game);
		DrawingWindow::addObject2D_to_Visual2D(r2, game);
		DrawingWindow::addObject2D_to_Visual2D(r3, game);
		DrawingWindow::addObject2D_to_Visual2D(r4, game);

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(offset_x, offset_y);
		Transform2D::applyTransform(r1);
		Transform2D::applyTransform(r2);
		Transform2D::applyTransform(r3);
		Transform2D::applyTransform(r4);
	}

	//mut inamicul in pozitia initiala
	void move_fourth_enemy(int width) {

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(width / 2, -80);
		Transform2D::applyTransform(r1);
		Transform2D::applyTransform(r2);
		Transform2D::applyTransform(r3);
		Transform2D::applyTransform(r4);
		offset_x = width / 2;
		offset_y = -60;
	}

	//eliminare din contextul vizual
	void remove(Visual2D *game) {

		DrawingWindow::removeObject2D_from_Visual2D(r1, game);
		DrawingWindow::removeObject2D_from_Visual2D(r2, game);
		DrawingWindow::removeObject2D_from_Visual2D(r3, game);
		DrawingWindow::removeObject2D_from_Visual2D(r4, game);
	}
};