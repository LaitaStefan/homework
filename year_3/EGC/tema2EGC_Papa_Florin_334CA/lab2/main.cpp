//Papa Florin, 334CA
#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Background.h"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <fstream>


#define PI 3.14159265358979323846
#define inf 1000000
using namespace std;

Visual2D *v2d1, *score, *score_color, *background_color;
clock_t init_clk, final_clk;


int hits = 5, time_elapsed = 0, sc = 0, finish = 0;
float n=1, maxSpeed = 30, mySpeed = 0, shift = 0, turn = 0, fuel = 100, dist = 3000;
int width = DrawingWindow::width, height = DrawingWindow::height;
int turnKey = 0, downKey = 0, upKey = 0;
Car *c;
Road *r;
Rectangle2D *p1, *p2; //folosite pentru afisare progres
Collisions *coll;	//clasa care se ocupa cu gestionarea inamicilor si a coliziunilor
Stripes *s;
Text *text1, *text2, *text4, *text3, *text_hits, *nr_hits;
Text *text_time, *text_clock, *text_fuel, *nr_fuel, *text_dist, *text_d;

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{	
	//initializare timp
	init_clk = clock();

	//contextul vizual al scorului
	score = new Visual2D(-width/2, -height/10, width/2,height/10,0,0,width,height/5);
	addVisual2D(score);

	//contextul vizual al jocului
	v2d1 = new Visual2D(-width/2,-height/2,width/2,height/2, 0,0,width,height); 
	addVisual2D(v2d1);

	//adaugare masina la contextul vizual
	c = new Car(70,70, Color(0,0,0), Color(1,1,1), true);
	c->add_objects(v2d1);

	//adaugare drum la contextul vizual
	r = new Road();
	addObject3D_to_Visual2D(r, v2d1);

	//adaugare dungi
	s = new Stripes();
	s->addAll_to_visual2D(v2d1);

	//adaugare scor
	text1 = new Text("SCORE", Point2D( (-15 * 5) / 2, height / 18), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	text2 = new Text("0", Point2D(-7, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);

	addText_to_Visual2D(text1, score);
	addText_to_Visual2D(text2, score);

	//adaugare distanta
	text_d = new Text("DISTANCE LEFT", Point2D( (-15 * 13) / 2, -height/36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text_d, score);

	//adaugare lovituri
	text_hits = new Text("HITS", Point2D(-width/2 + 20, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	nr_hits = new Text("5", Point2D(-width/2 + 90, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);

	addText_to_Visual2D(text_hits, score);
	addText_to_Visual2D(nr_hits, score);

	//adaugare timp
	text_time = new Text("TIME ELAPSED", Point2D(width/2 - 195, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text_time, score);

	//adaugare fuel
	text_fuel = new Text("FUEL", Point2D(-width/2 + 20, -height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text_fuel, score);

	//initializare coliziuni
	coll = new Collisions(hits);

	//adaugare culoare scor
	score_color = new Visual2D(-width/2, -height/10, width/2,height/10,0,0,width,height/5);
	score_color->cadruFereastra(Color(0,0.73,0.81));
	addVisual2D(score_color);

	//adaugare culoare fundal
	background_color = new Visual2D(-width/2,-height/2,width/2,height/2, 0,0,width,height);
	Background::add_objects_to_background(background_color, height);
	addVisual2D(background_color);

	//adaugare cursor progres
	p1 = new Rectangle2D(Point2D(-width/2 + 30, 0), 20, (5000 - dist) / 15, Color(0,1,0), true);
	p2 = new Rectangle2D(Point2D(-width/2 + 30, (5000 - dist) / 15), 20, dist/15, Color(0,1,0), false);
	DrawingWindow::addObject2D_to_Visual2D(p1, v2d1);
	DrawingWindow::addObject2D_to_Visual2D(p2, v2d1);
}

//functie care calculeaza rotatia si translatia masinii
void calculate_shift() {
	if(turn < PI/4 && turn > -PI/4) {
		turn += turnKey * 0.1;		
	}
	if(shift + 8*turnKey <= 120 && shift + 8*turnKey >= -120) shift += 8*turnKey;
	if(shift >= 119 || shift <= -119 || shift == 0) {
		turnKey = 0; 
		turn = 0;
	}
}

//functie care intoarce numarul de cifre al unui numar, in vederea centrarii pe ecran
int digit_number(int nr) {

	int x = 0, aux = nr;
	do {	
		aux = aux / 10;
		x ++;
	} while(aux > 0);

	return x;
}

//functie care intoarce ceasul in format string
string getFormat(int t) {
	string s = "";
	int aux = t;
	int sec = aux % 60;
	int min = aux / 60;
	int hr = aux / 3600;

	if(digit_number(hr) == 1) s += "0" + to_string(hr) + ":";
	else s+= to_string(hr) + ":";

	if(digit_number(min) == 1) s += "0" + to_string(min) + ":";
	else s+= to_string(min) + ":";

	if(digit_number(sec) == 1) s += "0" + to_string(sec);
	else s+= to_string(sec);

	return s;
}

//functie care afiseaza scor, timp, lovituri
void display_status() {
	final_clk = clock() - init_clk;
	double t = (double) final_clk / CLOCKS_PER_SEC;
	if(t > time_elapsed) time_elapsed = t;

	//afisare progres
	DrawingWindow::removeObject2D_from_Visual2D(p1, v2d1);
	DrawingWindow::removeObject2D_from_Visual2D(p2, v2d1);
	p1 = new Rectangle2D(Point2D(-width/2 + 30, -height/5), 20, (3000 - dist) / 10, Color(0,1,0), true);
	p2 = new Rectangle2D(Point2D(-width/2 + 30, (3000 - dist) / 10 - height/5), 20, dist/10, Color(0,1,0), false);
	DrawingWindow::addObject2D_to_Visual2D(p1, v2d1);
	DrawingWindow::addObject2D_to_Visual2D(p2, v2d1);

	//afisare scor centrat
	int pos = - 12 * digit_number(sc) / 2;
	DrawingWindow::removeText_from_Visual2D(text2, score);
	text2 = new Text(to_string(sc), Point2D(pos, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text2, score);

	//afisare distanta centrata
	pos = - 12 * digit_number(ceil(dist)) / 2;
	DrawingWindow::removeText_from_Visual2D(text_dist, score);
	text_dist = new Text(to_string((int)ceil(dist)), Point2D(pos, -height / 18), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_dist, score);
	
	//afisare numar lovituri
	DrawingWindow::removeText_from_Visual2D(nr_hits, score);
	nr_hits = new Text(to_string(hits), Point2D(-width/2 + 90, height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(nr_hits, score);

	//afisare timp
	DrawingWindow::removeText_from_Visual2D(text_clock, score);
	text_clock = new Text(getFormat(time_elapsed), Point2D(width/2 - 150, -height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_clock, score);

	//afisare carburant
	DrawingWindow::removeText_from_Visual2D(nr_fuel, score);
	nr_fuel = new Text(to_string((int) ceil(fuel)), Point2D(-width/2 + 90, -height / 36), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(nr_fuel, score);
}

//functie care afiseaza un mesaj daca am atins un record
void check_highscore() {
	ifstream in("highscore.txt");
	int scr = 0;
	in >> scr;
	in.close();

	ofstream out("highscore.txt");
	if(scr < sc) {
		text4 = new Text("ACHIEVED NEW HIGHSCORE!!!", Point2D(-15*24/2 + 500, height / 6 - 100), Color(0, 0, 0), BITMAP_TIMES_ROMAN_24); 
		DrawingWindow::addText_to_Visual2D(text4, v2d1);

		out << to_string(sc);
		out.close();
	} 
	else out.close();
}

void print_final_message() {
	text3 = new Text("GAME OVER", Point2D(-15 * 9/2 + 500, height / 6), Color(0,0,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text3, v2d1);
}

//functia care permite animatia
void DrawingWindow::onIdle()
{
	if(finish == 0) {
		calculate_shift();
		c->move_car(shift, turn, mySpeed);
		s->move(mySpeed);
		r->view();
		dist -= mySpeed / 100;

		//generare si mutare inamici
		coll->generate_enemies(v2d1, mySpeed);
		int col_status = coll->check_collisions(v2d1, new Point3D(shift, 0, 0));
		sc = coll->get_score();
		if(col_status == 0 || fuel <= 0 || dist <= 0) { //am terminat vietile
			finish = 1;
			print_final_message();
			check_highscore();
			coll->removeAll(v2d1);
		}
		if(col_status == -1) fuel = 100; //am lovit benzina
		else hits = col_status;

		display_status();

		//modificare viteza
		if(upKey == 1 && mySpeed + 0.2 <= maxSpeed) {
			mySpeed += 0.2;
		}
		else if(downKey == 1 && mySpeed - 0.4 >= 0) {
			mySpeed -= 0.4;
		}
		else if(mySpeed - 0.05 >= 0) mySpeed -= 0.05; 

		if(upKey == 1) fuel -= 0.015;
		else fuel -= 0.005;

		coll->moveEnemies(mySpeed);
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int _width,int _height)
{	
	width = _width;
	height = _height;

	v2d1->poarta(0,0,width,height);
	score->poarta(0,0,width,height/5);
	background_color->poarta(0, 0, width, height);
	score_color->poarta(0, 0, width, height/5);
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	switch(key)
	{
		case 27 : exit(0);
		case KEY_LEFT:
			turnKey = -1;
			break;
		case KEY_RIGHT:
			turnKey = 1;
			break;
		case KEY_UP:
			upKey = 1;
			break;
		case KEY_DOWN:
			downKey = 1;
			break;
	}
}

//functie care defineste ce se intampla la eliberarea unei taste
void DrawingWindow::onReleaseKey(unsigned char key) {
	if(key == KEY_UP) upKey = 0;
	if(key == KEY_DOWN) downKey = 0;
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1400, 800, 0, 0, "Tema 2 EGC");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;
}