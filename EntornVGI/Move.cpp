#include "stdafx.h"
#include "Move.h"

void Move::setMove_acc(int dir,  double vmax, double dur) {
	v_rotacional = 0;
	if (dir == 1)type = 1;
	else type = 0;
	duracio = dur;
	instant = 0;
	dir_rotacio = dir;
	is_libre = false;
	this->rot_acc = 1;

	this->v_max_rotacional = vmax;
}
void Move::setMove_freno(int dir,double dur) {
	dir_rotacio = dir;
	v_rotacional = 0;
	is_libre = false;
	type = 2;
	this->rot_acc = -1;
	duracio = dur;
}
void Move::setMove_wait(double dur) {
	v_rotacional = 0;
	type = 3;
	duracio = dur;
	is_libre = false;
}
void Move::setfreemove(double dur) {
	type = 4;
	dir_rotacio = 0;
	duracio = dur;
	v_rotacional = 0;
	is_libre = true;
}

void Move::move_step_rot(coord &angle_obj,double time) {
	if (!is_libre && type != 3) {
		// Control Sentit acceleracio
			//	Control X
		if (rot_acc == -1) {
			//frena
			v_rotacional = v_rotacional - acc_angular_freno*time;
			if (v_rotacional < 0) v_rotacional = 0;
		}
		else if (rot_acc == 1) {
			//gas
			if (abs(v_rotacional) > v_max_rotacional) {
				if (v_rotacional < 0)v_rotacional = -v_max_rotacional;
				else v_rotacional = v_max_rotacional;
			}
			else {
				v_rotacional = v_rotacional + acc_angular_plus*time;
			}
		}


		//moviment planeta1
		//moviment X+
		if (dir_rotacio == 1) {
			angle_obj.x = angle_obj.x + v_rotacional*time;

		}
		//moviment X-
		else if (dir_rotacio == -1) {
			angle_obj.x = angle_obj.x - v_rotacional*time;

		}
	}
}
