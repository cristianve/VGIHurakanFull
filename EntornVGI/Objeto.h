#pragma once
#include <iostream>
#include "Conj_Animacions.h"
class Objeto
{
public:
	Objeto() {

	}

	void del_Animacio() {
		moves.deleteQueue();
		delete(move_act);
	};

	void set_instant(double instant) {
		this->instant = instant;
	};
	double get_duracio() {
		return duracio;
	};

	coord get_angle() {
		return angle;
	};
	void set_angle(double angle_x, double angle_y, double angle_z) {
		angle.x = angle_x;
		angle.y = angle_y;
		angle.z = angle_z;
	};
	coord get_pos() {
		return pos;
	};
	void read_moves(char* filename);
	void set_pos(double x,double y,double z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	};
	void set_v(double x, double y, double z) {
		velo.v_X = x;
		velo.v_Y = y;
		velo.v_Z = z;
	};
	void set_v_angular(double x, double y, double z) {
		velo_angular.v_X = x;
		velo_angular.v_Y = y;
		velo_angular.v_Z = z;
	};
	void set_desp_origen(double x, double y, double z) {
		desp_origen.x = x;
		desp_origen.y = y;
		desp_origen.z = z;
	};
	coord get_desp_origen() {
		return desp_origen;
	};

	void step();

	void add_move(Move* m);
	
	

private:
	//variables control moviment
	coord desp_origen;
	coord angle;
	coord pos;

	v velo;
	v velo_angular;
	
	acc accel_dir;

	dir direc;
	
	MoveQueue moves;
	Move* move_act;

	//variables control temporal
	double instant = 0;
	double duracio = 0;
	
};

