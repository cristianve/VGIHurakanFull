#pragma once


// dir: 0 = no move ; 1 = + ; -1 = -
struct dir {
	double d_x = 0;
	double d_y = 0;
	double d_z = 0;
};
// 0: no_move		1: accelera		-1: frena
struct acc {
	int acc_x = 0;
	int acc_y = 0;
	int acc_z = 0;
};
//v max model
struct v {
	double v_X = 0;
	double v_Y = 0;
	double v_Z = 0;
};

struct coord {
	double x=0;
	double y=0;
	double z=0;
};


class Move
{
private:
	float g_plus = 0.008;
	float g_freno = 0.008;
	float acc_angular_plus = 0.008;
	float acc_angular_freno = 0.008;


	//0: Moviment lineal		1: moviment rotacional
	int type = 0;

	//mov rotacional
	dir dir_rotacio;
	acc rot_acc;
	v v_rotacional;
	v v_max_rotacional;


	dir move_dir;
	acc move_acc;
	v v_move;
	v v_move_max;
	double t_inici;
	double instant;
	double duracio;
	double t_final;

public:

	void move_step_rot(coord &a);
	void move_step_lin(coord &pos);

	bool is_finished() {
		return (instant >= t_final);
	};

	void set_instant(double inst) {
		instant = inst;
	}

	void set_t_ini(double t_inici) {
		t_final = t_inici + duracio/3.34;
		this->t_inici = t_inici;
	}

	void setMove_Lineal(int dir_x,int dir_y,int dir_z,int acc_x, int acc_y,int acc_z,double vmax_x,double vmax_y,double vmax_z,double dur) {
		type = 0;
		duracio = dur;
		instant = 0;
		dir move_dir;
		move_dir.d_x = dir_x;
		move_dir.d_y = dir_y;
		move_dir.d_z = dir_z;
		this->move_dir = move_dir;
		acc move_acc;
		move_acc.acc_x = acc_x;
		move_acc.acc_y = acc_y;
		move_acc.acc_z = acc_z;
		this->move_acc = move_acc;
		v v_move;
		v_move.v_X = vmax_x;
		v_move.v_Y = vmax_y;
		v_move.v_Z = vmax_z;
		this->v_move_max = v_move;
	};

	

	void setMove_Rotacional(int dir_x, int dir_y, int dir_z, int acc_x, int acc_y, int acc_z, double vmax_x, double vmax_y, double vmax_z, double dur) {
		type = 1;
		duracio = dur;
		instant = 0;
		dir move_dir;
		move_dir.d_x = dir_x;
		move_dir.d_y = dir_y;
		move_dir.d_z = dir_z;
		this->move_dir = move_dir;
		acc move_acc;
		move_acc.acc_x = acc_x;
		move_acc.acc_y = acc_y;
		move_acc.acc_z = acc_z;
		this->rot_acc = move_acc;
		v v_move;
		v_move.v_X = vmax_x;
		v_move.v_Y = vmax_y;
		v_move.v_Z = vmax_z;
		this->v_max_rotacional = v_move;
	};

	int get_type() {
		return type;
	};

	double get_t_final() {
		return t_final;
	};

	double getMoveDur() {
		return duracio;
	};

	dir getMoveDir() {
		if (type) {
			return dir_rotacio;
		}
		else {
			return move_dir;
		}
	};

	acc getMoveAcc() {
		if (type) {
			return rot_acc;
		}
		else {
			return move_acc;
		}
	};
	void setMoveV(v v_s) {
		if (type){
			v_rotacional = v_s;
		}
		else {
			v_move = v_s;
		}
	}
	v getMoveV() {
		if (type) {
			return v_rotacional;
		}
		else {
			return v_move;
		}
	};
	

};

