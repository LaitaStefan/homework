//Papa Florin, 334CA

#include <iostream>
#include "Circle2D.h"
#include "Rectangle2D.h"

class Enemy1 {
public:
	Circle2D *circle;
	Rectangle2D *rect;
	float offset_x, offset_y;
	int hits;

	Enemy1() {
		hits = 0;
	}

	//adaugare la contextul vizual
	void addFirstEnemy(Visual2D *game, int height) {

		circle = new Circle2D(Point2D(0, 0), 25, Color(0,1,0), true);
		rect = new Rectangle2D(Point2D(-25, -25), 50, 50, Color(1, 0, 0), true);
		offset_x = -60;
		offset_y = 5*height / 12;

		DrawingWindow::addObject2D_to_Visual2D(circle, game);
		DrawingWindow::addObject2D_to_Visual2D(rect, game);

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(offset_x, offset_y);
		Transform2D::applyTransform(circle);
		Transform2D::applyTransform(rect);
	}

	//mutare in pozitia initala
	void move_first_enemy(int height) {

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(-80, 5*height / 12);
		Transform2D::applyTransform(circle);
		Transform2D::applyTransform(rect);
		offset_x = -60;
		offset_y = 5*height / 12;
	}

	//eliminare din contextul vizual
	void remove(Visual2D *game) {

		DrawingWindow::removeObject2D_from_Visual2D(circle, game);
		DrawingWindow::removeObject2D_from_Visual2D(rect, game);
	}
};

