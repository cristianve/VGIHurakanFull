#include "stdafx.h"
#include "Objecte.h"

void Objecte::reset_moves() {
	while (!moves.empty()) {
		moves.pop_mov();
	}
	move_act = NULL;
}

void Objecte::step() {
	int d;
	double time = (instant - lastInstant);

	if (instant >= move_act->t_fi && moves.get_n_movs() == 1) {
		freeStep_f(time);
	}else if (instant>=move_act->t_fi && !moves.empty()) { //si el moviment ha acabat i hi ha algun a la cua
			
		//s'agafa el nou moviment de la cua
		moves.pop_mov();
		if (!moves.empty()) {
			move_act = moves.top();
			move_act->t_ini = instant;
			move_act->t_fi = instant + move_act->dur;
				
		}
	}
	//si el moviment no ha acabat pero el instant actual es major al seu temps d'inici
	else if (instant<move_act->t_fi && move_act->t_ini <= instant) {
			//avancem movimen
		switch (move_act->type)
		{
		case 'F':
			frenar(time,move_act->acc);
			break;
		case 'L':
			freeStep_f(time);
			break;
		case 'A':
			accelerar(time, true,move_act->vmax,move_act->acc);
			break;
		case 'Z':
			accelerar(time, false,move_act->vmax,move_act->acc);
			break;
		case 'C':
			if ((v_angular > MAXIMA_VELOCIDAD_CLAVAR) || (v_angular < (MAXIMA_VELOCIDAD_CLAVAR) * (-1)) || acceleracio > MAXIMA_ACELERACION_CLAVAR || acceleracio < (MAXIMA_ACELERACION_CLAVAR) * (-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
			{
				frenar(time,HURAKAN_FRENO);
			}
			else
			{
				v_angular = 0;
				acceleracio = 0;
			}
			break;
		case 'W':
			break;
		default:
			break;
		}
	}
	
}

void Objecte::stepTeclat()
{
	double time = (instant - lastInstant);
	switch (estat)
	{
		case FRENAR:
			frenar(time,HURAKAN_FRENO);
			break;
		case LIBRE:
			freeStep_f(time);
			break;
		case ACELERAR_POSITIVO:
			accelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
			break;
		case ACELERAR_NEGATIVO:
			accelerar(time, false,V_MAXIMA,HURAKAN_ACELERACION);
			break;
		case CLAVAR_BRAZO:
			if ((v_angular > MAXIMA_VELOCIDAD_CLAVAR) || (v_angular < (MAXIMA_VELOCIDAD_CLAVAR)*(-1)) || acceleracio > MAXIMA_ACELERACION_CLAVAR || acceleracio < (MAXIMA_ACELERACION_CLAVAR)*(-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
			{
				frenar(time,HURAKAN_FRENO);
			}
			else
			{
				v_angular = 0;
				acceleracio = 0;
			}
			break;
		case PAUSAR:
			break;
		default:
			break;
	}
	//Call Grabacio
	write_moves();
}

void Objecte::stepSeient()
{

}

void Objecte::stepSeientTeclat()
{
	double time = (instant - lastInstant);
	switch (estat)
	{
	case CLAVAR_ASIENTO:
		isTrontollarTimeSet = false;
		if ((v_angular > MAXIMA_VELOCIDAD_CLAVAR) || (v_angular < (MAXIMA_VELOCIDAD_CLAVAR) * (-1)) || acceleracio > MAXIMA_ACELERACION_CLAVAR || acceleracio < (MAXIMA_ACELERACION_CLAVAR) * (-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
		{
			frenar(time,HURAKAN_FRENO);
		}
		else
		{
			v_angular = 0;
			acceleracio = 0;
		}
		break;
	case LIBRE:
		isTrontollarTimeSet = false;
		freeStep_f(time);
		break;
	case GIRAR_POSITIVO:
		isTrontollarTimeSet = false;
		accelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
		break;
	case GIRAR_NEGATIVO:
		isTrontollarTimeSet = false;
		accelerar(time, false,V_MAXIMA,HURAKAN_ACELERACION);
		break;
	case TAMBALEAR:
		if(!isTrontollarTimeSet)
			startTrontollarTime = instant;
		trontollar(time);
		break;
	case PAUSAR:
		break;
	default:
		break;
	}
	//Call Grabacio
	write_moves();
}

void Objecte::trontollar(double time)
{
	isTrontollarTimeSet = true;
	double trontollarInstant = instant - startTrontollarTime;
	if(trontollarInstant <= 0.7)
	{ 
		accelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
	}
	else
	{
		freeStep_f(time);
	}
}

void Objecte::add_move(Move* m) {
	//actualitza duracio
	duracio += m->dur;
	//afegeix a la cua de moviments
	moves.push_mov(m);
	//actualitza l'actual i els seus par�metres.
	move_act = moves.top();
}

void Objecte::read_moves(char* filename,double instant) {
	FILE* movements_record;
	int n_moves = 0;
	char type = 'X';
	char aux = 'X';
	int n_llegits;
	double time = 0.0;
	double acc = 0.0;
	double vmax = 0.0;
	duracio = 0;
	this->instant = instant;

	movements_record = fopen(filename, "r");
	if (movements_record != NULL)
	{
		n_llegits = fscanf(movements_record, "%lf\n", &angle.x);
		n_llegits = fscanf(movements_record, "%lf\n", &v_angular);
		n_llegits = fscanf(movements_record, "%lf\n", &acceleracio);
		n_llegits = fscanf(movements_record, "%lf\n", &fricc);
		n_llegits = fscanf(movements_record, "%d\n", &n_moves);
		for (int i = 0; i < n_moves; i++) {
			n_llegits = fscanf(movements_record, "%c", &type);
			n_llegits = fscanf(movements_record, "%lf", &time);

			Move* m = new Move;
			switch (type)
			{
			case 'W':
				m->type = type;
				m->dur = time;
				break;
			case 'F':
				n_llegits = fscanf(movements_record, " %lf", &acc);
				m->type=type;
				m->dur = time;
				m->acc = acc;
				break;
			case 'A':
				n_llegits = fscanf(movements_record, " %lf %lf", &acc,&vmax);
				m->type = type;
				m->dur = time;
				m->acc = acc;
				m->vmax = vmax;
				break;
			case 'Z':
				n_llegits = fscanf(movements_record, " %lf %lf", &acc, &vmax);
				m->acc = acc;
				m->type = type;
				m->vmax = vmax;
				m->dur = time;
				break;
			case 'L':
				m->type = type;
				m->dur = time;
				break;
			case 'C':
				m->type = type;
				m->dur = time;
				break;
			default:
				break;
			}
			fscanf(movements_record, "\n");
			add_move(m);

		}
		moves.top()->t_ini = instant;
		moves.top()->t_fi = instant + moves.top()->dur;
		fclose(movements_record);
	}



}

void Objecte::freeStep_f(double time)
{
	bool izq = false;
	float actualAngle = angle_moviment_lliure;

	if (actualAngle > 180)
	{
		actualAngle = actualAngle - 180;
		izq = true;
	}
	

	float rozamiento = -fricc * v_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = (pes*9.82) * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	acceleracio = (fuerzaTotal / pes)*15;

	if (abs(acceleracio) > 90) {
		if (acceleracio > 0) {
			acceleracio = 90;
		}
		else {
			acceleracio = -90;
		}
	}
	v_angular += (acceleracio)*time;
	if (v_angular > V_MAXIMA) v_angular = V_MAXIMA;
	if (v_angular < -V_MAXIMA)v_angular = -V_MAXIMA;

	angle.x = angle.x + (v_angular * time);

	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objecte::accelerar(double time, bool isPositiu,double vmax,double accel)
{
	if (v_angular < -vmax) {
		v_angular = -vmax;
	}
	if (v_angular > vmax) {
		v_angular = vmax;
	}
	else {
		acceleracio = accel;
		if (!isPositiu)
			acceleracio *= (-1);
		v_angular += acceleracio * time;
	}
	angle.x = angle.x + (v_angular * time);
	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objecte::frenar(double time,double freno)
{
	if (v_angular > 0)
	{
		v_angular -= freno * time;
		if (v_angular < 0) v_angular = 0;
	}
	else
	{
		v_angular += freno * time;
		if (v_angular > 0) v_angular = 0;
	}
	angle.x = angle.x + (v_angular * time);
	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}
void Objecte::freeStep_b(double time)
{
	bool izq = true;
	float actualAngle = angle_moviment_lliure;

	if (actualAngle < -180)
	{
		actualAngle = actualAngle + 180;
		izq = false;
	}


	float rozamiento = -fricc * v_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = (pes*9.82) * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	acceleracio = (fuerzaTotal / pes)*15;
	if (abs(acceleracio) > 90) {
		if (acceleracio > 0) {
			acceleracio = 90;
		}
		else {
			acceleracio = -90;
		}
	}

	v_angular += (acceleracio)*time;
	if (v_angular > V_MAXIMA) v_angular = V_MAXIMA;
	if (v_angular < -V_MAXIMA)v_angular = -V_MAXIMA;
	angle.x = angle.x - (v_angular * time);

	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objecte::set_gravacio_filename(char* filename)
{
	if (!isGravant)
	{
		gravacioFilename = filename;
	}
}
void Objecte::setGravacio(bool gravacio)
{
	if (isGravant && !gravacio)		//Se acabó la gravación
	{
		tempsGravacio = instant - lastGravacioInstant;
		if (tempsGravacio > 0 && n_moves < NUM_OF_MOVEMENTS) { //Si hay un movimiento en curso y queda hueco en el array
			//Recoger en el array el ultimo movimiento, ya que queda suelto
			char state = getState();
			movements_record[n_moves] = state;
			movements_record_time[n_moves] = tempsGravacio;
		}

		//Escribir en el fichero
		FILE* gravacio;
		gravacio = fopen(gravacioFilename, "w");

		//Grabar posicion inicial
		fprintf(gravacio, "%lf\n", gravacio_angle_inicial);
		fprintf(gravacio, "%lf\n", gravacio_velocitat_inicial);
		fprintf(gravacio, "%lf\n", gravacio_acceleracio_inicial);
		fprintf(gravacio, "%lf\n", gravacio_friccio_inicial);

		//Grabar movimientos
		fprintf(gravacio, "%d\n", n_moves + 1);
		for (int i = 0; i < n_moves + 1; i++)
		{
			if (movements_record[i] != 'A' && movements_record[i] != 'Z' && movements_record[i] != 'F')
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					fprintf(gravacio, "%c%lf", movements_record[i], movements_record_time[i]);					//Si es el ultimo le quita el salto de linea
				}
				else {
					fprintf(gravacio, "%c%lf\n", movements_record[i], movements_record_time[i]);
				}
			}
			else
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					if (movements_record[i] == 'F')fprintf(gravacio, "%c%lf %d %d", movements_record[i], movements_record_time[i], HURAKAN_FRENO,V_MAXIMA-2);
					else fprintf(gravacio, "%c%lf %d %d", movements_record[i], movements_record_time[i], HURAKAN_ACELERACION,V_MAXIMA-2);					//Si es el ultimo le quita el salto de linea
				}
				else {
					if (movements_record[i] == 'F')fprintf(gravacio, "%c%lf %d %d\n", movements_record[i], movements_record_time[i], HURAKAN_FRENO,V_MAXIMA-2);
					else fprintf(gravacio, "%c%lf %d %d\n", movements_record[i], movements_record_time[i], HURAKAN_ACELERACION,V_MAXIMA-2);
				}
			}
		}
		fclose(gravacio);
	}
	if (!isGravant && gravacio)	//Se inicia la gravacio
	{
		recentIniciat = true;
		n_moves = 0; //Reiniciem els moviments
		tempsGravacio = 0;
		lastGravacioInstant = instant;

		//Guardamos la posicion inicial
		gravacio_angle_inicial = angle.x;
		gravacio_velocitat_inicial = v_angular;
		gravacio_acceleracio_inicial = acceleracio;
		gravacio_friccio_inicial = fricc;
	}
	isGravant = gravacio;
}

char Objecte::getState()
{
	char state = 'W';
	switch (lastEstat)
	{
	case CLAVAR_ASIENTO:
		state = 'C';
		break;
	case LIBRE:
		state = 'L';
		break;
	case GIRAR_POSITIVO:
		state = 'A';
		break;
	case GIRAR_NEGATIVO:
		state = 'Z';
		break;
	case TAMBALEAR:
		state = 'T';
		break;
	case PAUSAR:
		state = 'W';
		break;
	default:
		state = 'X';
		break;
	}
	return state;
}

void Objecte::write_moves()
{
	if (isGravant)
	{
		if (recentIniciat)
		{
			recentIniciat = false;
			lastEstat = estat;
		}
		if (lastEstat != estat)
		{
			tempsGravacio = instant - lastGravacioInstant;
			lastGravacioInstant = instant;
			char state = getState();
			movements_record[n_moves] = state;
			movements_record_time[n_moves] = tempsGravacio;
			n_moves++;
			tempsGravacio = 0;	//Reinicia el temps
			lastEstat = estat;
			if (n_moves == NUM_OF_MOVEMENTS)
			{
				setGravacio(false);		//si no hay mas hueco en el array finaliza la grabación
			}
		}
		else
		{
			//tempsGrabacion += 0.01; //Suma 10 ms
		}
	}
}