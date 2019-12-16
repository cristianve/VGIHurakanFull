
#include <iostream>
#include "Gestor_moviments.h"
#include "constants.h"


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
	double x = 0;
	double y = 0;
	double z = 0;
};

class Objecte
{
private:

	//GENERAL OBJECTE

	//posicio i desp
	coord pos;
	coord desp_origen;

	//angles
	coord angle;
	double angle_moviment_lliure = 0;

	//velocitat
	double v_angular = 0;
	
	//temps
	double instant = 0;
	double duracio = 0;


	//CONTROL DEMO
	MoveQueue moves;
	Move* move_act;
	


	//CONTROL FREE MOVE
	bool isBrac = false;
	bool islliure = true;
	float pes = 1000;
	double acceleracio = 0;
	double lastInstant = 0;
	double fricc = 0;

	//CONTROL TECLAT
	int estat = PAUSAR;
	//tambaleo
	bool isTrontollarTimeSet = false;
	double startTrontollarTime = 0;


	//CONTROL GRABACIO
	double tempsGravacio = 0;
	bool isGravant= false;
	int lastEstat = PAUSAR;
	char* gravacioFilename = NULL;
	int n_moves = 0;
	char movements_record[NUM_OF_MOVEMENTS];
	double movements_record_time[NUM_OF_MOVEMENTS];
	bool recentIniciat = false;
	double lastGravacioInstant = 0;

	double gravacio_angle_inicial;
	double gravacio_velocitat_inicial;
	double gravacio_acceleracio_inicial;
	double gravacio_friccio_inicial;

public:

	//Constructor
	Objecte() { angle.x = 0; };
	
	//GENERAL OBJECTE

	//posicio
	coord get_posicio() { return pos; };
	void set_posicio(double x, double y, double z) { pos.x = x; pos.y = y; pos.z = z; };
	void set_desp_origen(double x, double y, double z) { desp_origen.x = x; desp_origen.y = y; desp_origen.z = z; };
	coord get_desp_origen() { return desp_origen; };

	//angle
	coord get_angle() { return angle; };
	void set_angle(double angle_x, double angle_y, double angle_z) { angle.x = angle_x; angle.y = angle_y; angle.z = angle_z; };

	//velo
	void set_v_angular(double x) { v_angular = x; };

	//temps
	void set_instant(double instant) { this->lastInstant = this->instant; this->instant = instant; };
	double get_duracio() { return duracio; };

	//CONTROL DEMO
	void read_moves(char* filename, double instant);
	void add_move(Move* m);
	void step();
	void reset_moves();

	//CONTROL FREE MOVE
	void set_pes(double pes) { this->pes = pes; };
	void set_angle_moviment_lliure(double angle) { angle_moviment_lliure = angle; }
	double get_angle_moviment_lliure() { return angle_moviment_lliure; }
	void set_fricc(double fact) { fricc = fact; };
	void freeStep_f(double time);
	void freeStep_b(double time);
	
	//CONTROL MOVIMENTS
	void set_estat(int estat) { this->estat = estat; }
	void stepTeclat();
	void stepSeient();
	void stepSeientTeclat();
	void trontollar(double time);
	void accelerar(double time, bool isPositiu,double vmax,double accel);
	void frenar(double time,double freno);
	char getState();
	void set_gravacio_filename(char* filename);
	void setGravacio(bool gravacio);
	void write_moves();
	
};

