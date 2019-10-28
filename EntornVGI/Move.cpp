#include "stdafx.h"
#include "Move.h"

void Move::move_step_rot(coord &angle_obj) {
	// Control Sentit acceleracio
		//	Control X
	if (rot_acc.acc_x == -1) {
		//frena
		v_rotacional.v_X = v_rotacional.v_X - acc_angular_freno;
		if (v_rotacional.v_X < 0) v_rotacional.v_X = 0;
	}
	else if (rot_acc.acc_x == 1) {
		//gas
		if (v_rotacional.v_X > v_max_rotacional.v_X) {
			v_rotacional.v_X = v_max_rotacional.v_X;
		}
		else {
			v_rotacional.v_X = v_rotacional.v_X + acc_angular_plus;
		}
	}
	// Control Y
	if (rot_acc.acc_y == -1) {
		//frena
		v_rotacional.v_Y = v_rotacional.v_Y - acc_angular_freno;
		if (v_rotacional.v_Y < 0)v_rotacional.v_Y = 0;
	}
	else if (rot_acc.acc_y == 1){
		//gas
		if (v_rotacional.v_Y > v_max_rotacional.v_Y) {
			v_rotacional.v_Y = v_max_rotacional.v_Y;
		}
		else {
			v_rotacional.v_Y = v_rotacional.v_Y + acc_angular_plus;
		}
	}
	//	Control Z
	if (rot_acc.acc_z == -1) {
		//frena
		v_rotacional.v_Z = v_rotacional.v_Z - acc_angular_freno;
		if (v_rotacional.v_Z < 0) v_rotacional.v_Z = 0;
	}
	else if (rot_acc.acc_z == 1) {
		//gas
		if (v_rotacional.v_Z > v_max_rotacional.v_Z) {
			v_rotacional.v_Z = v_max_rotacional.v_Z;
		}
		else {
			v_rotacional.v_Z = v_rotacional.v_Z + acc_angular_plus;
		}
	}

	//moviment planeta1
	//moviment X+
	if (move_dir.d_x == 1) {
		angle_obj.x = angle_obj.x + v_rotacional.v_X;

	}
	//moviment X-
	else if (move_dir.d_x == -1) {
		angle_obj.x = angle_obj.x - v_rotacional.v_X;

	}
	//moviment Y+
	if (move_dir.d_y == 1) {
		angle_obj.y = angle_obj.y + v_rotacional.v_Y;

	}
	//moviment Y-
	else if (move_dir.d_y == -1) {
		angle_obj.y = angle_obj.y - v_rotacional.v_Y;
	}
	//moviment +Z
	if (move_dir.d_z == 1) {
		angle_obj.z = angle_obj.z + v_rotacional.v_Z;
	}
	//moviment -Z
	else if (move_dir.d_z == -1) {
		angle_obj.z = angle_obj.z - v_rotacional.v_Z;

	}
}

void Move::move_step_lin(coord &pos) {

	// Control Sentit acceleracio
	//	Control X
	if (move_acc.acc_x == -1) {
		//frena
		v_move.v_X = v_move.v_X - g_freno;
		if (v_move.v_X < 0) v_move.v_X = 0;
	}
	else if (move_acc.acc_x == 1) {
		//gas
		if (v_move.v_X > v_move_max.v_X) {
			v_move.v_X = v_move_max.v_X;
		}
		else {
			v_move.v_X = v_move.v_X + g_plus;
		}
	}
	// Control Y
	if (move_acc.acc_y == -1) {
		//frena
		v_move.v_Y = v_move.v_Y - g_freno;
		if (v_move.v_Y < 0)v_move.v_Y = 0;
	}
	else if (move_acc.acc_y ==1){
		//gas
		if (v_move.v_Y > v_move_max.v_Y) {
			v_move.v_Y = v_move_max.v_Y;
		}
		else {
			v_move.v_Y = v_move.v_Y + g_plus;
		}
	}
	//	Control Z
	if (move_acc.acc_z == -1) {
		//frena
		v_move.v_Z = v_move.v_Z - g_freno;
		if (v_move.v_Z < 0) v_move.v_Z = 0;
	}
	else if (move_acc.acc_z == 1) {
		//gas
		if (v_move.v_Z > v_move_max.v_Z) {
			v_move.v_Z = v_move_max.v_Z;
		}
		else {
			v_move.v_Z = v_move.v_Z + g_plus;
		}
	}

	//moviment planeta1
	//moviment X+
	if (move_dir.d_x == 1) {
		pos.x = pos.x + v_move.v_X;

	}
	//moviment X-
	else if (move_dir.d_x == -1) {
		pos.x = pos.x - v_move.v_X;

	}
	//moviment Y+
	if (move_dir.d_y == 1) {
		pos.y = pos.y + v_move.v_Y * 10;

	}
	//moviment Y-
	else if (move_dir.d_y == -1) {
		pos.y = pos.y - v_move.v_Y * 10;
	}
	//moviment +Z
	if (move_dir.d_z == 1) {
		pos.z = pos.z + v_move.v_Z * 4;
	}
	//moviment -Z
	else if (move_dir.d_z == -1) {
		pos.z = pos.z - v_move.v_Z * 4;

	}
	
}