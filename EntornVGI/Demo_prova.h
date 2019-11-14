#pragma once
#include "visualitzacio.h"
#include "Objeto.h"
#include "stdafx.h"
#include "constants.h"
#include <iostream>
#include <ctime>

#define LIBRE 1

class Demo_prova
{

private:
	float t = 0;
	bool textura = false;
	Objeto brac;
	Objeto seient;
	
public:
	int mode = 0;
	double interval = 10; // en ms (no cal canviar, resolucio de l'animaci�(afecta a la velocitat))
	double duracion_total; // en sec
	double instant = 0;
	std::clock_t start;
	std::clock_t t_base;
	std::clock_t t_pausa;
	double dur_pausa;
	bool pausa = false;
	bool demo_on;
	
	Demo_prova()
	{
		demo_on = false;

		//Iniciamos el brazo
		brac.set_estado(LIBRE);
		brac.set_desp_origen(0.5, 0, 8.94);
		brac.set_angle(0, 0, 0);
		brac.set_v_angular(0);

		//Iniciamos los asientos
		seient.set_estado(LIBRE);
		seient.set_desp_origen(0.5, 0, 3);
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
	void dibuixa_inicial(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);
	void reset_demo();
	clock_t get_t_base() {return t_base;}
	void set_t_base() {t_base = std::clock();}
	void set_start() {start = std::clock();}
	clock_t get_start() {return start;}

	void start_demo();
	void dibuixa_demo1(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);
	void move_step();
};
