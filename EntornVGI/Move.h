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
	double acc_angular_plus = 10;
	double acc_angular_freno = 20;

	int type = 0;

	//mov rotacional
	int dir_rotacio;
	int rot_acc;
	double v_rotacional;
	double v_max_rotacional;

	//control temporal
	double t_inici;
	double instant;
	double duracio;
	double t_final;

	

public:
	
	bool is_libre;

	//inicialitzar animacions
	void setMove_acc(int dir, double vmax, double dur);
	void setMove_freno(int dir,double dur);
	void setMove_wait(double dur);
	void setfreemove(double dur);

	//set temps inicial
	void set_t_ini(double t_inici) { t_final = t_inici + duracio; this->t_inici = t_inici; };
	//get temps inicial
	double get_t_ini() { return t_inici; };


	//avançar animacio
	void move_step_rot(coord &a,double time);
	//control final animacio
	bool is_finished() { return (instant >= t_final); };

	//avanç temporal animacio
	void set_instant(double inst) { instant = inst; };
	
	//GETS i SETS parametres:
	//ACC
	void set_acc(double acc) { acc_angular_plus = acc; };
	void set_freno(double acc) { acc_angular_freno = acc; };
	double get_acc() { return acc_angular_plus; };
	double get_freno() { return acc_angular_freno; };

	//Temps
	double get_t_final() { return t_final; };
	double getMoveDur() { return duracio; };

	//DIR
	void setMoveDir(double d) { dir_rotacio = d; };
	int getMoveDir() { return dir_rotacio; };
	int getMoveAccDir() { return rot_acc; };

	//Velo
	void setMoveV(double v_s) { v_rotacional = v_s; };
	double getMoveV() { return v_rotacional; };
	void setMoveVMax(double v_max) { v_max_rotacional = v_max; };
	double getMoveVMax() { return v_max_rotacional; };

	//Type
	int get_type() { return type; };

};

