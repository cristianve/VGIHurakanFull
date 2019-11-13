#pragma once
#include <iostream>
#include "MoveQueue.h"
class Objeto
{
public:
	Objeto() {
		peso = 200;
		isLibre = false;
	}

	void del_Animacio() {
		moves.deleteQueue();
		delete(move_act);
	};

	void set_instant(double instant) {
		this->lastInstant = this->instant;
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
	void read_moves(char* filename,double instant);
	void set_pos(double x,double y,double z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	};
	void set_v(double x) {
		velo = x;
	};
	void set_v_angular(double x) {
		velo_angular = x;
	};
	void set_desp_origen(double x, double y, double z) {
		desp_origen.x = x;
		desp_origen.y = y;
		desp_origen.z = z;
	};
	coord get_desp_origen() {
		return desp_origen;
	};
	void set_angle_abs(double angle) {
		angle_abs = angle;
	}

	void step();

	void add_move(Move* m);
	void freeStep(double time);
	
	

private:
	//variables control moviment
	coord desp_origen;
	coord angle;
	double angle_abs;
	coord pos;

	double velo;
	double velo_angular;
	
	double accel_dir;

	double direc;
	
	MoveQueue moves;
	Move* move_act;

	//variables control temporal
	double instant = 0;
	double duracio = 0;

	//Variables para el modo libre 
	float peso;
	double aceleracion;
	bool isLibre;
	double lastInstant = 0;
	
};

