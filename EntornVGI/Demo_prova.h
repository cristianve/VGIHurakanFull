#pragma once
#include "visualitzacio.h"
#include "Objeto.h"
#include "stdafx.h"
#include "constants.h"
#include <iostream>
#include <ctime>

class Demo_prova
{

private:
	float t = 0;
	
	bool textura = false;

	
	Objeto brac;
	Objeto seient;
	
	

public:
	
	int mode = 0;
	double interval = 10; // en ms (no cal canviar, resolucio de l'animació(afecta a la velocitat))
	double duracion_total; // en sec
	double instant = 0;
	std::clock_t start;
	std::clock_t t_pausa;
	double dur_pausa;
	bool pausa = false;
	
	
	Demo_prova() {
	
	};
	Demo_prova(float t, float angle_1) {
		
	};
	void dibuixa_inicial();



	clock_t get_start() {
		return start;
	}

	void start_demo() {
		//inicialitzant direccio moviments
		brac.read_moves("brac_moves.txt");
		seient.read_moves("seient_moves.txt");
		dur_pausa = 0;
		start = std::clock();
		brac.set_desp_origen(0.5, 0, 8.94);
		seient.set_desp_origen(0.5, 0, 3);
		brac.set_angle(0, 0, 0);
		seient.set_angle(0, 0, 0);
		instant = 0;
		brac.set_v_angular(0, 0, 0);
		seient.set_v_angular(0, 0, 0);
		duracion_total = brac.get_duracio();
		//ini_movs();


	};

	void dibuixa_demo1();

	void move_step();


};
