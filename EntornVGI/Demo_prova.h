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
	std::clock_t t_base;
	std::clock_t t_pausa;
	double dur_pausa;
	bool pausa = false;
	bool demo_on;
	
	
	Demo_prova() {
		demo_on = false;
	};
	Demo_prova(float t, float angle_1) {
		
	};
	void dibuixa_inicial(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);

	void reset_demo() {
		brac.set_angle(0, 0, 0);
		seient.set_angle(0, 0, 0);
		brac.set_v_angular(0);
		seient.set_v_angular(0);
	};

	clock_t get_t_base() {
		return t_base;
	}
	void set_t_base() {
		t_base = std::clock();
	}
	void set_start() {
		start = std::clock();
	}
	clock_t get_start() {
		return start;
	}

	void start_demo() {
		set_start();
		demo_on = true;
		//inicialitzant direccio moviments
		brac.read_moves("brac_moves.txt",instant);
		seient.read_moves("seient_moves.txt",instant);
		dur_pausa = 0;
		
		brac.set_desp_origen(0.5, 0, 8.94);
		seient.set_desp_origen(0.5, 0, 3);
		brac.set_angle(0, 0, 0);
		seient.set_angle(0, 0, 0);
		brac.set_v_angular(0);
		seient.set_v_angular(0);
		duracion_total = brac.get_duracio();
		//ini_movs();


	};

	void dibuixa_demo1(bool textu, GLint VTextu[NUM_MAX_TEXTURES]);

	void move_step();


};
