//Papa Florin, 334CA
#include "Framework\Visual2D.h"
#include "Framework\Line2D.h"
#include "Framework\Polygon2D.h"
#include "Road.h"
#include "Collisions.h"
#include "Stripes.h"

class Background {
public:
	static void add_objects_to_background(Visual2D *background_color, int height) {
		Polygon2D *color_poly;
		Line2D *delim1, *delim2, *delim3, *delim4, *delim5, *delim6;

		color_poly = new Polygon2D(Color(0.5, 0.5, 0.5), true);
		color_poly->addPoint(Point2D(-468, -height / 2));
		color_poly->addPoint(Point2D(468, -height / 2));
		color_poly->addPoint(Point2D(0, height / 2 - 150));
		color_poly->addPoint(Point2D(0, height / 2 - 150));
		delim1 = new Line2D(Point2D(-469, -height/2), Point2D(0, height/2 -150), Color(0,0,0));
		delim2 = new Line2D(Point2D(-468, -height/2), Point2D(1, height/2 -150), Color(0,0,0));
		delim3 = new Line2D(Point2D(-467, -height/2), Point2D(1, height/2 -150), Color(0,0,0));
		delim4 = new Line2D(Point2D(469, -height/2), Point2D(1, height/2 -150), Color(0,0,0));
		delim5 = new Line2D(Point2D(468, -height/2), Point2D(1, height/2 -150), Color(0,0,0));
		delim6 = new Line2D(Point2D(467, -height/2), Point2D(0, height/2 -150), Color(0,0,0));
		DrawingWindow::addObject2D_to_Visual2D(delim1, background_color);
		DrawingWindow::addObject2D_to_Visual2D(delim2, background_color);
		DrawingWindow::addObject2D_to_Visual2D(delim3, background_color);
		DrawingWindow::addObject2D_to_Visual2D(delim4, background_color);
		DrawingWindow::addObject2D_to_Visual2D(delim5, background_color);
		DrawingWindow::addObject2D_to_Visual2D(delim6, background_color);
		DrawingWindow::addObject2D_to_Visual2D(color_poly, background_color);
		background_color->cadruFereastra(Color(1, 0.9, 0.57));
	}
};