#pragma once
#include <iostream>
#include "MoveQueue.h"

#define FRENAR 0
#define LIBRE 1
#define ACELERAR_POSITIVO 2
#define ACELERAR_NEGATIVO 3
#define CLAVAR_BRAZO 4
#define PAUSAR 5

#define CLAVAR_ASIENTO 0
#define LIBRE 1
#define GIRAR_POSITIVO 2
#define GIRAR_NEGATIVO 3
#define TAMBALEAR 4

#define HURAKAN_ACELERACION 10
#define HURAKAN_FRENO 15

#define MAXIMA_VELOCIDAD_CLAVAR 0.5
#define MAXIMA_ACELERACION_CLAVAR 1
#define MAXIMA_VELOCIDAD_ACELERACION (30)

class Objeto
{
public:
	Objeto() 
	{
		peso = 200;
		isLibre = false;
		estado = PAUSAR;
		isTambaleoTimeSet = false;
	}

	void del_Animacio() {
		moves.deleteQueue();
		delete(move_act);
	};

	void set_instant(double instant) 
	{
		this->lastInstant = this->instant;
		this->instant = instant;
	};
	double get_duracio() {return duracio;};

	coord get_angle() {return angle;};
	void set_angle(double angle_x, double angle_y, double angle_z) {
		angle.x = angle_x;
		angle.y = angle_y;
		angle.z = angle_z;
	};

	coord get_pos() {return pos;};
	void set_pos(double x, double y, double z)
	{
		pos.x = x;
		pos.y = y;
		pos.z = z;
	};

	void read_moves(char* filename,double instant);

	void set_v(double x) {velo = x;};
	void set_v_angular(double x) {velo_angular = x;};
	void set_desp_origen(double x, double y, double z) 
	{
		desp_origen.x = x;
		desp_origen.y = y;
		desp_origen.z = z;
	};
	coord get_desp_origen() {return desp_origen;};
	void set_angle_abs(double angle) {angle_abs = angle;}

	void set_estado(int estado) { this->estado = estado; }

	void step();
	void stepTeclado();
	void stepAsiento();
	void stepAsientoTeclado();
	void tambaleo(double time);
	void add_move(Move* m);
	void freeStep_f(double time);
	void freeStep_b(double time);
	

	void acelerar(double time, bool isPositivo);
	void frenar(double time);
private:
	//variables control moviment
	coord desp_origen;
	coord angle;
	double angle_abs;
	coord pos;

	bool isTambaleoTimeSet;
	double startTambaleoTime;
	double velo;
	double velo_angular;
	
	double accel_dir;

	double direc;
	
	MoveQueue moves;
	Move* move_act;

	//variables control temporal
	double instant = 0;
	double duracio = 0;

	//Variables para el modo libre 
	float peso;
	double aceleracion;
	bool isLibre;
	double lastInstant = 0;
	bool isBrazo;

	//Variables modo teclado
	int estado;
};

