//Papa Florin, 334CA

#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Framework/Enemy1.h"
#include "Framework/Enemy2.h"
#include "Framework/Enemy3.h"
#include "Framework/Enemy4.h"
#include <iostream>
#include <windows.h>
#include <fstream>


#define PI 3.14159265358979323846

using namespace std;

int width = DrawingWindow::width, height = DrawingWindow::height;
int score = 0, lives = 5, level = 1, final = 0, bonus_level = 0, bonus = 0;
int upKey, rightKey, leftKey, fireKey, downKey;

float pasRot = 0, translX = width / 2, translY = 5 * height / 12, enemy_speed = 0.05;
float player_speed = 0.6, rot_speed = 0.05, bonusX, bonusY;;

Visual2D *game, *score_frame, *background;
Rectangle2D *rectangle, *rectangle2;
Circle2D *player_circle, *life_circle, *bonus_circle;
Polygon2D *player_poly, *life_poly, *bonus_poly;
Enemy1 *enemy1;
Enemy2 *enemy2;
Enemy3 *enemy3;
Enemy4 *enemy4;

vector<Polygon2D*> bullets;
vector<Point2D*> bullet_offset;
vector<float> bullet_rot;

Text *text1, *text2, *text3, *text4, *text_level, *text_life;
Line2D *line, *line2;

//functie care returneaza distanta dintre centrele a 2 obiecte
float distance(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

//functia care verifica daca jucatorul a iesit din teren
int possible_player() {
	if(translX + 10*cos(pasRot) < width && 
		translY + 10*sin(pasRot) < 5 * height / 6 &&
		translX + 10*cos(pasRot) > 0 && 
		translY + 10*sin(pasRot) > 0) return 1;

	return 0;
}

//functie care descrie comportarea gloantelor
int possible_bullet(int i) {
	//a iesit din fereastra
	if(bullet_offset[i]->x + cos(bullet_rot[i]) >= width &&
		bullet_offset[i]->x + cos(bullet_rot[i]) <= 0 &&
		bullet_offset[i]->y + sin(bullet_rot[i]) >= 5 * height / 6 &&
		bullet_offset[i]->y + sin(bullet_rot[i]) <= 0) return 0;

	//s-a lovit de primul inamic
	else if(distance(bullet_offset[i]->x, 
			bullet_offset[i]->y			, 
			enemy1->offset_x			, 
			enemy1->offset_y) <= 30) return 1;

	//s-a lovit de al doilea inamic
	else if(distance(bullet_offset[i]->x, 
			bullet_offset[i]->y			, 
			enemy2->offset_x			, 
			enemy2->offset_y) <= 30) return 2;

	//s-a lovit de al treilea inamic
	else if(distance(bullet_offset[i]->x, 
			bullet_offset[i]->y			, 
			enemy3->offset_x			, 
			enemy3->offset_y) <= 30) return 3;

	//s-a lovit de al patrulea inamic
	else if(distance(bullet_offset[i]->x, 
			bullet_offset[i]->y			, 
			enemy4->offset_x			, 
			enemy4->offset_y) <= 30) return 4;

	//s-a lovit de bonus
	else if(distance(bullet_offset[i]->x, 
			bullet_offset[i]->y			, 
			bonusX						, 
			bonusY) <= 30 && bonus == 1) return 5;

	return 6;	//nu s-a lovit de nimic
}

//miscarea inamicilor
void moveEnemies() {
	//primul inamic
	if(enemy1->offset_x < translX) enemy1->offset_x += enemy_speed;
	else if(enemy1->offset_x > translX) enemy1->offset_x -= enemy_speed;

	if(enemy1->offset_y < translY) enemy1->offset_y += enemy_speed;
	else if(enemy1->offset_y > translY) enemy1->offset_y -= enemy_speed;

	//al doilea
	if(enemy2->offset_x < translX) enemy2->offset_x += enemy_speed;
	else if(enemy2->offset_x > translX) enemy2->offset_x -= enemy_speed;

	if(enemy2->offset_y < translY) enemy2->offset_y += enemy_speed;
	else if(enemy2->offset_y > translY) enemy2->offset_y -= enemy_speed;

	//al treilea
	if(enemy3->offset_x < translX) enemy3->offset_x += enemy_speed;
	else if(enemy3->offset_x > translX) enemy3->offset_x -= enemy_speed;

	if(enemy3->offset_y < translY) enemy3->offset_y += enemy_speed;
	else if(enemy3->offset_y > translY) enemy3->offset_y -= enemy_speed;

	//al patrulea
	if(enemy4->offset_x < translX) enemy4->offset_x += enemy_speed;
	else if(enemy4->offset_x > translX) enemy4->offset_x -= enemy_speed;

	if(enemy4->offset_y < translY) enemy4->offset_y += enemy_speed;
	else if(enemy4->offset_y > translY) enemy4->offset_y -= enemy_speed;

	//mut obiectele 
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(enemy1->offset_x, enemy1->offset_y);
	Transform2D::applyTransform(enemy1->circle);
	Transform2D::applyTransform(enemy1->rect);

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(enemy2->offset_x, enemy2->offset_y);
	Transform2D::applyTransform(enemy2->c1);
	Transform2D::applyTransform(enemy2->c2);

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(enemy3->offset_x, enemy3->offset_y);
	Transform2D::applyTransform(enemy3->face);
	Transform2D::applyTransform(enemy3->eye1);
	Transform2D::applyTransform(enemy3->eye2);
	Transform2D::applyTransform(enemy3->line);

	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(enemy4->offset_x, enemy4->offset_y);
	Transform2D::applyTransform(enemy4->r1);
	Transform2D::applyTransform(enemy4->r2);
	Transform2D::applyTransform(enemy4->r3);
	Transform2D::applyTransform(enemy4->r4);
}

//functia care trateaza evenimentele cauzate de gloante
void treat_bullet_action() {
	//gloantele sunt mutate sau eliminate daca au iesit din fereastra
	for(int i = 0; i < bullets.size(); i++) {

		if(possible_bullet(i) == 6) { //se poate continua
			Transform2D::loadIdentityMatrix();
			Transform2D::rotateMatrix(bullet_rot[i] - PI / 2);

			bullet_offset[i]->x += 4 * cos(bullet_rot[i]);
			bullet_offset[i]->y += 4 * sin(bullet_rot[i]);

			Transform2D::translateMatrix(bullet_offset[i]->x, bullet_offset[i]->y);
			Transform2D::applyTransform(bullets[i]);
		}
		else {
			if(possible_bullet(i) == 1) { //am lovit primul obiect
				//adaug la numarul de lovituri primite
				enemy1->hits ++;

				if(enemy1->hits == 2) { //la 2 lovituri enemy1 "moare"
					enemy1->hits = 0;
					score += level * 10;

					//mut obiectul inapoi in pozitia initiala
					enemy1->move_first_enemy(height);
				}
			}

			if(possible_bullet(i) == 2) { //am lovit al doilea obiect
				enemy2->hits ++;

				if(enemy2->hits == 4) { //la 4 lovituri "moare" enemy2
					enemy2->hits = 0;
					score += level * 25;

					//mut obiectul inapoi in pozitia initiala
					enemy2->move_second_enemy(width, height);
				}
			}

			if(possible_bullet(i) == 3) { //am lovit al treilea obiect
				enemy3->hits ++;

				if(enemy3->hits == 6) { //la 6 lovituri "moare" enemy3
					enemy3->hits = 0;
					score += level * 50;

					//mut obiectul inapoi in pozitia initiala
					enemy3->move_third_enemy(width, height);
				}
			}

			if(possible_bullet(i) == 4) { //am lovit al patrulea obiect
				enemy4->hits ++;

				if(enemy4->hits == 8) { //la 8 lovituri "moare" enemy4
					enemy4->hits = 0; 
					score += level * 75;

					//mut obiectul inapoi in pozitia initiala
					enemy4->move_fourth_enemy(width);
				}
			}

			if(possible_bullet(i) == 5 && bonus == 1) { //am lovit bonusul
				bonus = 0;
				lives ++;
				DrawingWindow::removeObject2D_from_Visual2D(bonus_circle, game);
				DrawingWindow::removeObject2D_from_Visual2D(bonus_poly, game);
			}

			DrawingWindow::removeObject2D_from_Visual2D(bullets[i], game);
			bullets.erase(bullets.begin() + i);
			bullet_rot.erase(bullet_rot.begin() + i);
			bullet_offset.erase(bullet_offset.begin() + i);
			i--;
		}
	}
}

int check_collisions() {

	//jucatorul a fost lovit de primul inamic
	if(distance(translX, translY, enemy1->offset_x, enemy1->offset_y) < 70) {
		lives --;
		if(lives == 0) return 1;

		enemy1->move_first_enemy(height);
	}

	//jucatorul a fost lovit de al doilea inamic
	if(distance(translX, translY, enemy2->offset_x, enemy2->offset_y) < 70) {
		lives --;
		if(lives == 0) return 1;

		enemy2->move_second_enemy(width, height);
	}

	//jucatorul a fost lovit de al treilea inamic
	if(distance(translX, translY, enemy3->offset_x, enemy3->offset_y) < 70) {
		lives --;
		if(lives == 0) return 1;

		enemy3->move_third_enemy(width, height);
	}

	//jucatorul a fost lovit de al patrulea inamic
	if(distance(translX, translY, enemy4->offset_x, enemy4->offset_y) < 70) {
		lives --;
		if(lives == 0) return 1;

		enemy4->move_fourth_enemy(width);
	}

	//jucatorul a lovit bonusul
	if(distance(translX, translY, bonusX, bonusY) < 70 && bonus == 1) {
		lives ++;
		bonus = 0;
		DrawingWindow::removeObject2D_from_Visual2D(bonus_circle, game);
		DrawingWindow::removeObject2D_from_Visual2D(bonus_poly, game);
	}

	return 0;
}

void remove_all_objects() {
	enemy1->remove(game);
	enemy2->remove(game);
	enemy3->remove(game);
	enemy4->remove(game);

	DrawingWindow::removeObject2D_from_Visual2D(player_circle, game);
	DrawingWindow::removeObject2D_from_Visual2D(player_poly, game);

	if(bonus == 1) {
		DrawingWindow::removeObject2D_from_Visual2D(bonus_circle, game);
		DrawingWindow::removeObject2D_from_Visual2D(bonus_poly, game);
	}

	for(int i = 0; i < bullets.size(); i++) {
		DrawingWindow::removeObject2D_from_Visual2D(bullets[i], game);
	}
}

void print_final_message() {
	text3 = new Text("GAME OVER", Point2D((width - 15 * 9) / 2, 5 * height / 12), Color(0,1,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text3, game);
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

//functia care afiseaza scorul curent
void print_all(int s, int life, int lev) {
	int pos = (width - 12 * digit_number(s)) / 2;

	//afisare scor centrat
	DrawingWindow::removeText_from_Visual2D(text2, score_frame);
	text2 = new Text(to_string(s), Point2D(pos, height / 18), Color(0,1,0), BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text2, score_frame);

	//afisare nivel
	DrawingWindow::removeText_from_Visual2D(text_level, score_frame);
	text_level = new Text("LEVEL " + to_string(level), Point2D(width / 28, height / 12), Color(0, 1, 0), GLUT_BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_level, score_frame);

	//afisare vieti
	DrawingWindow::removeText_from_Visual2D(text_life, score_frame);
	text_life = new Text(to_string(lives) + " X ", Point2D(8*width / 9, height / 12), Color(0, 1, 0), GLUT_BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_life, score_frame);
}

void check_highscore() {
	ifstream in("highscore.txt");
	int sc = 0;
	in >> sc;
	printf("%d %d\n", sc, score);
	in.close();

	ofstream out("highscore.txt");

	if(sc < score) {
		text4 = new Text("ACHIEVED NEW HIGHSCORE!!!", Point2D((width - 15*24) / 2, 4*height / 12), Color(0, 1, 0), BITMAP_TIMES_ROMAN_24); 
		DrawingWindow::addText_to_Visual2D(text4, game);

		out << score;
		out.close();
	} 
	else out.close();
}

//functie care creeaza obiectul bonus
void create_bonus() {
	bonus_circle = new Circle2D(Point2D(0, 0), 50, Color(1,0,0), false);
	bonus_poly = new Polygon2D(Color(1,0,0), true);
	bonus_poly->addPoint(Point2D(-10, 0));
	bonus_poly->addPoint(Point2D(-35, 30));
	bonus_poly->addPoint(Point2D(50, 0));
	bonus_poly->addPoint(Point2D(-35, -30));
}

//functie care animeaza bonusul
void animate_bonus() {
	if(bonusY > 0) {
		bonusY -= 0.07;
		Transform2D::loadIdentityMatrix();
		Transform2D::scaleMatrix(0.5, 0.5);
		Transform2D::translateMatrix(bonusX, bonusY);
		Transform2D::applyTransform(bonus_circle);
		Transform2D::applyTransform(bonus_poly);
	}

	else {
		bonus = 0;
		DrawingWindow::removeObject2D_from_Visual2D(bonus_circle, game);
		DrawingWindow::removeObject2D_from_Visual2D(bonus_poly, game);
	}
}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	width = DrawingWindow::width;
	height = DrawingWindow::height;

	//contextul vizual al scorului si vietilor
	score_frame = new Visual2D(0, 0,width,height / 6,0, 0, width, height / 6);  
	addVisual2D(score_frame);

	//contextul vizual al jocului
	game = new Visual2D(0,0,width, height / 6 * 5,0, height / 6 , width, height);
	addVisual2D(game);
	
	//linie de separatie intre spatiul de joc si scor
	line = new Line2D(Point2D(0,0), Point2D(width, 0), Color(1, 0, 0));
	line2 = new Line2D(Point2D(0, 1), Point2D(width, 1), Color(1, 0, 0));
	addObject2D_to_Visual2D(line, score_frame);
	addObject2D_to_Visual2D(line2, score_frame);

	//adaugam jucatorul
	player_circle = new Circle2D(Point2D(0, 0), 50, Color(0,1,0), false);
	player_poly = new Polygon2D(Color(0,1,0), true);
	player_poly->addPoint(Point2D(-10, 0));
	player_poly->addPoint(Point2D(-35, 30));
	player_poly->addPoint(Point2D(50, 0));
	player_poly->addPoint(Point2D(-35, -30));
	addObject2D_to_Visual2D(player_circle, game);
	addObject2D_to_Visual2D(player_poly, game);

	//translatam jucatorul in centrul terenului
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(translX, translY);
	Transform2D::applyTransform(player_circle);
	Transform2D::applyTransform(player_poly);

	//adaugam bonusul
	create_bonus();

	//adaugam scorul, vietile si nivelul
	text1 = new Text("SCORE", Point2D((width - 15 * 5) / 2, 2 * height / 18), Color(0,1,0), BITMAP_TIMES_ROMAN_24);
	text2 = new Text("0", Point2D((width - 18) / 2, height / 18), Color(0,1,0), BITMAP_TIMES_ROMAN_24);

	addText_to_Visual2D(text1, score_frame);
	addText_to_Visual2D(text2, score_frame);

	text_level = new Text("LEVEL " + to_string(level), Point2D(width / 28, height / 12), Color(0, 1, 0), GLUT_BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_level, score_frame);

	text_life = new Text(to_string(lives) + " X ", Point2D(15*width / 17, 3*height / 32), Color(0, 1, 0), GLUT_BITMAP_TIMES_ROMAN_24);
	DrawingWindow::addText_to_Visual2D(text_life, score_frame);
	life_circle = new Circle2D(Point2D(0, 0), 50, Color(0,1,0), false);
	life_poly = new Polygon2D(Color(0,1,0), true);
	life_poly->addPoint(Point2D(-10, 0));
	life_poly->addPoint(Point2D(-35, 30));
	life_poly->addPoint(Point2D(50, 0));
	life_poly->addPoint(Point2D(-35, -30));
	addObject2D_to_Visual2D(life_circle, score_frame);
	addObject2D_to_Visual2D(life_poly, score_frame);
	Transform2D::loadIdentityMatrix();
	Transform2D::scaleMatrix(0.5, 0.5);
	Transform2D::translateMatrix(16*width/ 17, 3*height / 32);
	Transform2D::applyTransform(life_circle);
	Transform2D::applyTransform(life_poly);

	//adaugam primul inamic
	enemy1 = new Enemy1();
	enemy1->addFirstEnemy(game, height);

	//adaugam al doilea inamic
	enemy2 = new Enemy2();
	enemy2->addSecondEnemy(game, width, height);

	//adaugam al treilea inamic
	enemy3 = new Enemy3();
	enemy3->addThirdEnemy(game, width, height);

	//adaugam al patrulea inamic
	enemy4 = new Enemy4();
	enemy4->addFourthEnemy(game, width);

	//adaugam fundalul
	background = new Visual2D(0,0,width,height, 0, 0, width, height);
	background->cadruFereastra(Color(0, 0, 0));
	addVisual2D(background);
}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	if(final == 0) //nu am ajuns la finalul vietilor
	{
		//miscare inainte
		if(upKey == 1) {
			//se verifica daca obiectul a iesit din fereastra de afisare
			if(possible_player()) {
				//se muta jucatorul daca se poate
				Transform2D::loadIdentityMatrix();
				Transform2D::rotateMatrix(pasRot);
				Transform2D::translateMatrix(translX + player_speed*cos(pasRot), translY + player_speed*sin(pasRot));
				Transform2D::applyTransform(player_poly);
				Transform2D::applyTransform(player_circle);

				translX += player_speed * cos(pasRot);
				translY += player_speed * sin(pasRot);
			}
		}

		//miscare inapoi
		if(downKey == 1) {
			//se verifica daca obiectul a iesit din fereastra de afisare
			if(possible_player()) {
				//se muta jucatorul daca se poate
				Transform2D::loadIdentityMatrix();
				Transform2D::rotateMatrix(pasRot);
				Transform2D::translateMatrix(translX - player_speed*cos(pasRot), translY - player_speed*sin(pasRot));
				Transform2D::applyTransform(player_poly);
				Transform2D::applyTransform(player_circle);

				translX -= player_speed * cos(pasRot);
				translY -= player_speed * sin(pasRot);
			}
		}

		//rotatie dreapta
		if(rightKey == 1) {
			pasRot -= 0.1 * rot_speed;
			Transform2D::loadIdentityMatrix();
			Transform2D::rotateMatrix(pasRot);
			Transform2D::translateMatrix(translX, translY);
			Transform2D::applyTransform(player_circle);
			Transform2D::applyTransform(player_poly);
		}

		//rotatie stanga
		if(leftKey == 1) {
			pasRot += 0.1 * rot_speed;
			Transform2D::loadIdentityMatrix();
			Transform2D::rotateMatrix(pasRot);
			Transform2D::translateMatrix(translX, translY);
			Transform2D::applyTransform(player_circle);
			Transform2D::applyTransform(player_poly);
		}

		//la apasarea tastei SPACE, se creeaza "gloante"
		if(fireKey == 1) {
			Polygon2D *p1 = new Polygon2D(Color(0,1,0), true);
			Polygon2D *p2 = new Polygon2D(Color(0,1,0), true);

			p1->addPoint(Point2D(1, 0));
			p1->addPoint(Point2D(9, 0));
			p1->addPoint(Point2D(5, 10));

			p2->addPoint(Point2D(-1, 0));
			p2->addPoint(Point2D(-9, 0));
			p2->addPoint(Point2D(-5, 10));

			addObject2D_to_Visual2D(p1, game);
			addObject2D_to_Visual2D(p2, game);

			bullets.push_back(p1);
			bullets.push_back(p2);

			bullet_offset.push_back(new Point2D(translX, translY));
			bullet_offset.push_back(new Point2D(translX, translY));

			bullet_rot.push_back(pasRot);
			bullet_rot.push_back(pasRot);

			fireKey = 0; 
		}

		//tratare evenimente gloante
		treat_bullet_action();

		//modificare nivel
		if(score / 1000 + 1 > level) {
			level ++;
			player_speed += 0.15;
			enemy_speed += 0.007;
			rot_speed += 0.006;
		}

		//mutare inamici
		moveEnemies();

		//verificare coliziuni
		if(check_collisions()) { //jucatorul a pierdut ultima viata
			remove_all_objects();
			print_final_message();
			final = 1;

			check_highscore();
		}

		//verificare bonus
		if(bonus_level < score / 5000) {
			bonus_level++;
			bonus = 1; //variabila care indica existenta unui bonus pe ecran
			
			float poz = 0;
			while(poz < 30 || poz > width - 30) poz = rand() % width;

			bonusX = poz;
			bonusY = 5 * height / 6 + 80;

			addObject2D_to_Visual2D(bonus_circle, game);
			addObject2D_to_Visual2D(bonus_poly, game);
		}
	
		if(bonus == 1) {
			animate_bonus();
		}

		//afisare scor, level si vieti
		print_all(score, level, lives);
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width_,int height_)
{
	width = width_;
	height = height_;

	game->poarta(0, height_ / 6, width_, height_);
	score_frame->poarta(0, 0, width_, height_ / 6);
	background->poarta(0, 0, width_, height_);

}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{	
	if(key == KEY_UP) upKey = 1;
	if(key == 103) downKey = 1;
	if(key == KEY_LEFT) leftKey = 1;
	if(key == KEY_RIGHT) rightKey = 1;
	if(key == 32) fireKey = 1;

	if(key == 27) exit(0);
}

void DrawingWindow::onReleaseKey(unsigned char key) {

	if(key == KEY_UP) upKey = 0;
	if(key == 103) downKey = 0;
	if(key == KEY_LEFT) leftKey = 0;
	if(key == KEY_RIGHT) rightKey = 0;
	if(key == 32) fireKey = 0;
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1400, 800, 0, 0, "Tema 1 EGC");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}