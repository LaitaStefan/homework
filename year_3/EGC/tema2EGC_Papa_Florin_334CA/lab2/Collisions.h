//Papa Florin, 334CA
#pragma once

#include "WireSphere.h"
#include "Cube.h"
#include "FuelCylinder.h"
#include "Tethra.h"
#include "Car.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Framework\Point3D.h"
#include "Framework\Object3D.h"

class Collisions {
public:
	vector<Object3D*> enemies;
	vector<Point3D*> enemy_offset;
	vector<Car*> enemyCars;
	vector<Point3D*> car_offset;
	FuelCylinder *fuel;
	vector<Point3D*> fuel_offset;
	int hits, pas, score; 
	float timp_local;	//in functie de timp generez noi obiecte
	clock_t init_clk, final_clk;

public:
	Collisions(int nr_hits) {
		timp_local = 0;
		pas = 1;
		hits = nr_hits;
		score = 0;
		init_clk = clock();
		srand(time(NULL));
	}

	int check_collisions(Visual2D *v, Point3D *my_pos) {
		for(int i = 0; i < enemies.size(); i++) {
			int removed = 0;
			if(enemy_offset[i]->z >= 395 && 
				my_pos->x < enemy_offset[i]->x + 35 && 
				my_pos->x > enemy_offset[i]->x - 35) { // s-a produs o coliziune cu un inamic
					DrawingWindow::removeObject3D_from_Visual2D(enemies[i], v);
					enemies.erase(enemies.begin() + i);
					enemy_offset.erase(enemy_offset.begin() + i);
					i--;
					hits--;
					removed = 1;
					if(hits == 0) return hits;
			}
			//daca nu s-a lovit de inamic si inamicul a iesit din campul vizual, il elimin
			if(removed == 0 && enemy_offset[i]->z >= 500) {
				DrawingWindow::removeObject3D_from_Visual2D(enemies[i], v);
				enemies.erase(enemies.begin() + i);
				enemy_offset.erase(enemy_offset.begin() + i);
				i--;
				score += 25;
			}
		}

		for(int i = 0; i < enemyCars.size(); i++) {
			int removed = 0;
			if(car_offset[i]->z >= 395 && 
				my_pos->x < car_offset[i]->x + 35 && 
				my_pos->x > car_offset[i]->x - 35) { // s-a produs o coliziune cu o alta masina
					enemyCars[i]->remove_objects(v);
					enemyCars.erase(enemyCars.begin() + i);
					car_offset.erase(car_offset.begin() + i);
					i--;
					hits--;
					removed = 1;
					if(hits == 0) return hits;
			}
			//daca nu s-a lovit de inamic si inamicul a iesit din campul vizual, il elimin
			if(removed == 0 && car_offset[i]->z >= 500) {
				enemyCars[i]->remove_objects(v);
				enemyCars.erase(enemyCars.begin() + i);
				car_offset.erase(car_offset.begin() + i);
				i--;
				score += 35;
			}
		}

		if(fuel_offset.size() > 0 && fuel_offset[0]->z >= 395 && 
				my_pos->x < fuel_offset[0]->x + 35 && 
				my_pos->x > fuel_offset[0]->x - 35) { // s-a produs o coliziune cu canistra de benzina
					fuel->remove_objects(v);
					fuel_offset.erase(fuel_offset.begin());
					return -1;	//1 inseamna ca am lovit o canistra
		}
		//elimin benzina daca a iesit din campul vizual
		if(fuel_offset.size() > 0 && fuel_offset[0]->z >= 500) {
					fuel->remove_objects(v);
					fuel_offset.erase(fuel_offset.begin());
		}

		return hits; //nu am lovit nimic
	}

	int get_score() {
		return score;
	}

	void generate_enemies(Visual2D *v, float speed) {
		final_clk = clock() - init_clk;
		float t = (float) final_clk / CLOCKS_PER_SEC;
		if(speed != 0 && t > timp_local) { //generez obiecte doar daca ma misc
			timp_local += 0.5;
			int n = rand() % 4;
			int x = -120;
			int num = rand() % 3;
			if(n == 0) { //generez un tetraedru
				enemies.push_back(new Tethra(70, Color(0.94, 0.66, 0.56)));
				enemy_offset.push_back(new Point3D(x + num * 120, 0, -13000));
				DrawingWindow::addObject3D_to_Visual2D(enemies[enemies.size() - 1], v);
			}
			if(n == 1) { //generez un cub
				enemies.push_back(new Cube(70, Color(0,0,1)));
				enemy_offset.push_back(new Point3D(x + num * 120, 0, -13000));
				DrawingWindow::addObject3D_to_Visual2D(enemies[enemies.size() - 1], v);
			}
			if(n == 2) { //generez o sfera
				enemies.push_back(new WireSphere(35, Color(0.81, 1, 0.09)));
				enemy_offset.push_back(new Point3D(x + num * 120, 0, -13000));
				DrawingWindow::addObject3D_to_Visual2D(enemies[enemies.size() - 1], v);
			}
			if(n == 2) { //generez o masina
				enemyCars.push_back(new Car(70, 70, Color(1,0,0), Color(0,0,0)));
				car_offset.push_back(new Point3D(x + num * 120, 0, -13000));
				enemyCars[enemyCars.size() - 1]->add_objects(v);
			}

			if(timp_local > 90*pas) { //generez benzina
				pas++;
				fuel = new FuelCylinder(35, 80);
				fuel_offset.push_back(new Point3D(x + (120*((num + 1) % 3)), 0, -13000));
				fuel->add_objects(v);
			}
		}
		else if(t > timp_local) timp_local += 0.5;
	}

	void moveEnemies(float speed) {
		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(0,250,760);

		for(int i = 0; i < enemies.size(); i++) {
			enemy_offset[i]->z += speed;
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(enemy_offset[i]->x, 0, enemy_offset[i]->z);
			Transform3D::applyTransform(enemies[i]);
		}

		for(int i = 0; i < enemyCars.size(); i++) {
			car_offset[i]->z += speed - 15;
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(car_offset[i]->x, 0, car_offset[i]->z);
			Transform3D::applyTransform(enemyCars[i]->body);
			Transform3D::applyTransform(enemyCars[i]->windows);
			Transform3D::applyTransform(enemyCars[i]->w1);
			Transform3D::applyTransform(enemyCars[i]->w2);
			Transform3D::applyTransform(enemyCars[i]->w3);
			Transform3D::applyTransform(enemyCars[i]->w4);
		}

		if(fuel_offset.size() > 0) {
			fuel_offset[0]->z += speed;
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(fuel_offset[0]->x, 0, fuel_offset[0]->z);
			Transform3D::applyTransform(fuel->lower);
			Transform3D::applyTransform(fuel->upper);
		}
	}

	void removeAll(Visual2D *v) {
		for(int i = 0; i < enemies.size(); i++) {
			DrawingWindow::removeObject3D_from_Visual2D(enemies[i], v);
			enemies.erase(enemies.begin() + i);
			enemy_offset.erase(enemy_offset.begin() + i);
			i--;
		}

		for(int i = 0; i < enemyCars.size(); i++) {
			enemyCars[i]->remove_objects(v);
			enemyCars.erase(enemyCars.begin() + i);
			car_offset.erase(car_offset.begin() + i);
			i--;
		}

		if(fuel_offset.size() > 0) {
			fuel->remove_objects(v);
			fuel_offset.erase(fuel_offset.begin());
		}
	}

};
