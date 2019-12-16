#pragma once
#include "visualitzacio.h"
#include "Objecte.h"
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cmath>


class Hurakan
{

private:
	float t = 0;
	bool textura = false;
	Objecte brac;
	Objecte seient;
	double angle_abs_brac;
	 
public:

	//GESTIO TEMPORAL
	//Timer Entorn
	double interval = 10;// en ms (no cal canviar, resolucio de l'animaci�(afecta a la velocitat))
	//Ultima entrada al timer
	double last_instant = 0;
	double duracio_total; // en sec
	double instant = 0;
	std::clock_t start;
	std::clock_t t_base;
	std::clock_t t_pausa;
	double dur_pausa;
	bool pausa = false;

	//CONTROL
	bool demo_on;
	bool play_grabacio = false;

	//CAMARA SEIENTS
	CEsfe3D cap_seient;
	double pan_beta = 0;
	double pan_alfa = 0;
	
	Hurakan();

	void setEstatBrac(int estado) { brac.set_estat(estado); }
	void setEstatSeients(int estado) { seient.set_estat(estado); }
	CEsfe3D get_cap_seient() { return cap_seient; };
	double get_angle_brac() { return brac.get_angle().x; };
	void get_pos_seients(double& pos_x, double& pos_y, double& pos_z);
	int get_estat_brazo() { return brac.getState(); };
	int get_estat_Asiento() { return seient.getState(); };
	void reset_demo();
	clock_t get_t_base() {return t_base;}
	void set_t_base() {t_base = std::clock();}
	void set_start() {start = std::clock();}
	clock_t get_start() {return start;}

	void start_demo(char* brac_moves,char*seient_moves);
	void dibuixa_hurakan(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);
	void move_step();

	//Metodos grabacion
	void setGravacio();
	void initGravacio();
	void stopGravacio();
};
