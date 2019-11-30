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

#define HURAKAN_ACELERACION 30
#define HURAKAN_FRENO 20

#define MAXIMA_VELOCIDAD_CLAVAR 0.5
#define MAXIMA_ACELERACION_CLAVAR 1
#define MAXIMA_VELOCIDAD_ACELERACION (45)

#define NUM_OF_MOVEMENTS 1000

class Objeto
{
private:

	//GENERAL OBJECTE

	//posicio i desp
	coord pos;
	coord desp_origen;

	//angles
	coord angle;
	double angle_free_move = 0;

	//velocitat
	bool set_velo = false;
	double velo_angular = 0;
	
	//temps
	double instant = 0;
	double duracio = 0;


	//CONTROL DEMO
	MoveQueue moves;
	Move* move_act;
	double direc = 0;
	double accel_dir = -1;


	//CONTROL FREE MOVE
	bool isBrazo = false;
	bool isLibre = true;
	float peso = 1000;
	double aceleracion = 0;
	double lastInstant = 0;
	double fricc = 0;

	//CONTROL TECLAT
	int estado = PAUSAR;
	//tambaleo
	bool isTambaleoTimeSet = false;
	double startTambaleoTime = 0;


	//CONTROL GRABACIO
	double tiempoGrabacion = 0;
	bool isGrabando= false;
	int lastEstado = PAUSAR;
	char* grabacioFilename = NULL;
	int numberOfMovements = 0;
	char movements_record[NUM_OF_MOVEMENTS];
	double movements_record_time[NUM_OF_MOVEMENTS];
	bool recienIniciado = false;
	double lastGrabacioInstant = 0;

public:

	//Constructor
	Objeto() {};
	
	//GENERAL OBJECTE

	//posicio
	coord get_pos() { return pos; };
	void set_pos(double x, double y, double z) { pos.x = x; pos.y = y; pos.z = z; };
	void set_desp_origen(double x, double y, double z) { desp_origen.x = x; desp_origen.y = y; desp_origen.z = z; };
	coord get_desp_origen() { return desp_origen; };

	//angle
	coord get_angle() { return angle; };
	void set_angle(double angle_x, double angle_y, double angle_z) { angle.x = angle_x; angle.y = angle_y; angle.z = angle_z; };

	//velo
	void set_v_angular(double x) { velo_angular = x; set_velo = true; };

	//temps
	void set_instant(double instant) { this->lastInstant = this->instant; this->instant = instant; };
	double get_duracio() { return duracio; };

	//CONTROL DEMO
	void read_moves(char* filename, double instant);
	void add_move(Move* m);
	void step();
	void reset_moves();

	//CONTROL FREE MOVE
	void set_peso(double peso) { this->peso = peso; };
	void set_angle_free_move(double angle) { angle_free_move = angle; }
	void set_fricc(double fact) { fricc = fact; };
	void freeStep_f(double time);
	void freeStep_b(double time);
	
	//CONTROL TECLADO
	void set_estado(int estado) { this->estado = estado; }
	void stepTeclado();
	void stepAsiento();
	void stepAsientoTeclado();
	void tambaleo(double time);
	void acelerar(double time, bool isPositivo);
	void frenar(double time);
	char getState();
	void set_grabacio_filename(char* filename);
	void setGrabacio(bool grabacio);
	void write_moves();
	
};

