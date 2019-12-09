#pragma once
#include "visualitzacio.h"
#include "Objeto.h"
#include "stdafx.h"
#include "constants.h"
#include <iostream>
#include <ctime>
#include <cmath>

#define LIBRE 1

class Demo_prova
{

private:
	float t = 0;
	bool textura = false;
	Objeto brac;
	Objeto seient;
	double angle_abs_brac;
	 
public:
	int mode = 0;
	double interval = 10;
	double last_instant = 0;// en ms (no cal canviar, resolucio de l'animaci�(afecta a la velocitat))
	double duracion_total; // en sec
	double instant = 0;
	std::clock_t start;
	std::clock_t t_base;
	std::clock_t t_pausa;
	double dur_pausa;
	bool pausa = false;
	bool demo_on;
	bool play_grabacio = false;
	CEsfe3D cap_seient;
	CEsfe3D cap_brac;
	double pan_beta = 0;
	double pan_alfa = 0;
	
	Demo_prova()
	{
		angle_abs_brac = 0;
		cap_seient.alfa = 0;
		cap_seient.beta = 0;
		cap_brac.alfa = 0;
		cap_brac.beta = 0;
		demo_on = false;
		seient.set_peso(7000);
		brac.set_fricc(12);
		seient.set_fricc(8);
		//Iniciamos el brazo
		brac.set_estado(LIBRE);
		brac.set_desp_origen(DESP_ORIGEN_BRAC_X, DESP_ORIGEN_BRAC_Y, DESP_ORIGEN_BRAC_Z);
		brac.set_angle(0, 0, 0);
		brac.set_v_angular(0);

		//Iniciamos los asientos
		seient.set_estado(LIBRE);
		seient.set_desp_origen(DESP_ORIGEN_ASIENTO_X,DESP_ORIGEN_ASIENTO_Y,DESP_ORIGEN_ASIENTO_Z);
		seient.set_angle(0, 0, 0);
		seient.set_v_angular(0);
		if (brac.get_duracio() > seient.get_duracio()) duracion_total = brac.get_duracio();
		else duracion_total = seient.get_duracio();
		//ini_movs();

		//Iniciamos el clock
		set_start();
	};
	void setEstadoBrazo(int estado) { brac.set_estado(estado); }
	void setEstadoAsientos(int estado) { seient.set_estado(estado); }
	CEsfe3D get_cap_seient() { return cap_seient; };
	double get_angle_brac() { return brac.get_angle().x; };
	void get_pos_asientos(double& pos_x, double& pos_y, double& pos_z);
	void dibuixa_inicial(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);
	int get_estado_brazo() { return brac.getState(); };
	int get_estado_Asiento() { return seient.getState(); };
	void reset_demo();
	clock_t get_t_base() {return t_base;}
	void set_t_base() {t_base = std::clock();}
	void set_start() {start = std::clock();}
	clock_t get_start() {return start;}

	void start_demo(char* brac_moves,char*seient_moves);
	void dibuixa_demo1(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);
	void move_step();

	//Metodos grabacion
	void Demo_prova::setGrabacio();
	void Demo_prova::initGrabacio();
	void Demo_prova::stopGrabacio();
};
