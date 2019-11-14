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
	//variables acceleracio, es modifiquen amb set_acc i set_freno
	float g_plus = 2;
	float g_freno = 2;
	double acc_angular_plus = 10;
	double acc_angular_freno = 2;

	//0: Moviment lineal		1: moviment rotacional
	int type = 0;

	//mov rotacional
	int dir_rotacio;
	int rot_acc;
	double v_rotacional;
	double v_max_rotacional;

	
	//mov lineal
	int move_dir;
	int move_acc;
	double v_move;
	double v_move_max;
	

	//control temporal
	double t_inici;
	double instant;
	double duracio;
	double t_final;

	

public:
	bool is_libre;
	//inicialitzar animacions
	//void setMove_Lineal(int dir_x, int dir_y, int dir_z, int acc_x, int acc_y, int acc_z, double vmax_x, double vmax_y, double vmax_z, double dur);
	void setMove_acc(int dir, double vmax, double dur);
	void setMove_freno(int dir,double dur);
	void setMove_wait(double dur);
	void setfreemove(double dur);
	//avan�ar animacio
	
	void move_step_rot(coord &a,double time);
	//void move_step_lin(coord &pos);
	//control final animacio
	bool is_finished() {
		return (instant >= t_final);
	};


	//avan� temporal animacio
	void set_instant(double inst) {
		instant = inst;
	}
	//set temps inicial
	void set_t_ini(double t_inici) {
		t_final = t_inici + duracio;
		this->t_inici = t_inici;
	}
	//get temps inicial
	double get_t_ini() {
		return t_inici;
	};
	//funcions de canviar la acc
	void set_acc(double acc) {
		acc_angular_plus = acc;
	};
	void set_freno(double acc) {
		acc_angular_freno = acc;
	};
	//funcions de canviar la acc
	double get_acc() {
		return acc_angular_plus;
	};
	double get_freno() {
		return acc_angular_freno;
	};
	//obtenir tipus de la animacio(random)
	int get_type() {
		return type;
	};
	//get temps final
	double get_t_final() {
		return t_final;
	};
	//get duracio moviment
	double getMoveDur() {
		return duracio;
	};
	void setMoveDir(double d) {
		dir_rotacio = d;
	}
	int getMoveDir() {
		return dir_rotacio;
	};
	int getMoveAcc() {
		return rot_acc;
	};
	void setMoveV(double v_s) {
		v_rotacional = v_s;
	}
	double getMoveV() {
		return v_rotacional;
		
	};
	

};

