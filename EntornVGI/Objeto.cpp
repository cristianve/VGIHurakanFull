#include "stdafx.h"
#include "Objeto.h"

void Objeto::reset_moves() {
	while (!moves.empty()) {
		moves.pop_mov();
	}
	move_act = NULL;
}

void Objeto::step() {
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
			acelerar(time, true,move_act->vmax,move_act->acc);
			break;
		case 'Z':
			acelerar(time, false,move_act->vmax,move_act->acc);
			break;
		case 'C':
			if ((velo_angular > MAXIMA_VELOCIDAD_CLAVAR) || (velo_angular < (MAXIMA_VELOCIDAD_CLAVAR) * (-1)) || aceleracion > MAXIMA_ACELERACION_CLAVAR || aceleracion < (MAXIMA_ACELERACION_CLAVAR) * (-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
			{
				frenar(time,HURAKAN_FRENO);
			}
			else
			{
				velo_angular = 0;
				aceleracion = 0;
			}
			break;
		case 'W':
			break;
		default:
			break;
		}
	}
	
}

void Objeto::stepTeclado()
{
	double time = (instant - lastInstant);
	switch (estado)
	{
		case FRENAR:
			frenar(time,HURAKAN_FRENO);
			break;
		case LIBRE:
			freeStep_f(time);
			break;
		case ACELERAR_POSITIVO:
			acelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
			break;
		case ACELERAR_NEGATIVO:
			acelerar(time, false,V_MAXIMA,HURAKAN_ACELERACION);
			break;
		case CLAVAR_BRAZO:
			if ((velo_angular > MAXIMA_VELOCIDAD_CLAVAR) || (velo_angular < (MAXIMA_VELOCIDAD_CLAVAR)*(-1)) || aceleracion > MAXIMA_ACELERACION_CLAVAR || aceleracion < (MAXIMA_ACELERACION_CLAVAR)*(-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
			{
				frenar(time,HURAKAN_FRENO);
			}
			else
			{
				velo_angular = 0;
				aceleracion = 0;
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

void Objeto::stepAsiento()
{

}

void Objeto::stepAsientoTeclado()
{
	double time = (instant - lastInstant);
	switch (estado)
	{
	case CLAVAR_ASIENTO:
		isTambaleoTimeSet = false;
		if ((velo_angular > MAXIMA_VELOCIDAD_CLAVAR) || (velo_angular < (MAXIMA_VELOCIDAD_CLAVAR) * (-1)) || aceleracion > MAXIMA_ACELERACION_CLAVAR || aceleracion < (MAXIMA_ACELERACION_CLAVAR) * (-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
		{
			frenar(time,HURAKAN_FRENO);
		}
		else
		{
			velo_angular = 0;
			aceleracion = 0;
		}
		break;
	case LIBRE:
		isTambaleoTimeSet = false;
		freeStep_f(time);
		break;
	case GIRAR_POSITIVO:
		isTambaleoTimeSet = false;
		acelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
		break;
	case GIRAR_NEGATIVO:
		isTambaleoTimeSet = false;
		acelerar(time, false,V_MAXIMA,HURAKAN_ACELERACION);
		break;
	case TAMBALEAR:
		if(!isTambaleoTimeSet)
			startTambaleoTime = instant;
		tambaleo(time);
		break;
	case PAUSAR:
		break;
	default:
		break;
	}
	//Call Grabacio
	write_moves();
}

void Objeto::tambaleo(double time)
{
	isTambaleoTimeSet = true;
	double tambaleoInstant = instant - startTambaleoTime;
	if(tambaleoInstant <= 0.7)
	{ 
		acelerar(time, true,V_MAXIMA,HURAKAN_ACELERACION);
	}
	else
	{
		freeStep_f(time);
	}
}

void Objeto::add_move(Move* m) {
	//actualitza duracio
	duracio += m->dur;
	//afegeix a la cua de moviments
	moves.push_mov(m);
	//actualitza l'actual i els seus par�metres.
	move_act = moves.top();
}

void Objeto::read_moves(char* filename,double instant) {
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
		n_llegits = fscanf(movements_record, "%lf\n", &velo_angular);
		n_llegits = fscanf(movements_record, "%lf\n", &aceleracion);
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

void Objeto::freeStep_f(double time)
{
	bool izq = false;
	float actualAngle = angle_free_move;

	if (actualAngle > 180)
	{
		actualAngle = actualAngle - 180;
		izq = true;
	}
	

	float rozamiento = -fricc * velo_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = (peso*9.82) * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	aceleracion = (fuerzaTotal / peso)*15;

	if (abs(aceleracion) > 90) {
		if (aceleracion > 0) {
			aceleracion = 90;
		}
		else {
			aceleracion = -90;
		}
	}
	velo_angular += (aceleracion)*time;
	if (velo_angular > V_MAXIMA) velo_angular = V_MAXIMA;
	if (velo_angular < -V_MAXIMA)velo_angular = -V_MAXIMA;

	angle.x = angle.x + (velo_angular * time);

	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objeto::acelerar(double time, bool isPositivo,double vmax,double accel)
{
	if (velo_angular < -vmax) {
		velo_angular = -vmax;
	}
	if (velo_angular > vmax) {
		velo_angular = vmax;
	}
	else {
		aceleracion = accel;
		if (!isPositivo)
			aceleracion *= (-1);
		velo_angular += aceleracion * time;
	}
	angle.x = angle.x + (velo_angular * time);
	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objeto::frenar(double time,double freno)
{
	if (velo_angular > 0)
	{
		velo_angular -= freno * time;
		if (velo_angular < 0) velo_angular = 0;
	}
	else
	{
		velo_angular += freno * time;
		if (velo_angular > 0) velo_angular = 0;
	}
	angle.x = angle.x + (velo_angular * time);
	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}
void Objeto::freeStep_b(double time)
{
	bool izq = true;
	float actualAngle = angle_free_move;

	if (actualAngle < -180)
	{
		actualAngle = actualAngle + 180;
		izq = false;
	}


	float rozamiento = -fricc * velo_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = (peso*9.82) * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	aceleracion = (fuerzaTotal / peso)*15;
	if (abs(aceleracion) > 90) {
		if (aceleracion > 0) {
			aceleracion = 90;
		}
		else {
			aceleracion = -90;
		}
	}

	velo_angular += (aceleracion)*time;
	if (velo_angular > V_MAXIMA) velo_angular = V_MAXIMA;
	if (velo_angular < -V_MAXIMA)velo_angular = -V_MAXIMA;
	angle.x = angle.x - (velo_angular * time);

	if (angle.x <= -360) {
		angle.x = angle.x + 360;
	}
	else if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objeto::set_grabacio_filename(char* filename)
{
	if (!isGrabando)
	{
		grabacioFilename = filename;
	}
}
void Objeto::setGrabacio(bool grabacio)
{
	if (isGrabando && !grabacio)		//Se acabó la gravación
	{
		tiempoGrabacion = instant - lastGrabacioInstant;
		if (tiempoGrabacion > 0 && numberOfMovements < NUM_OF_MOVEMENTS) { //Si hay un movimiento en curso y queda hueco en el array
			//Recoger en el array el ultimo movimiento, ya que queda suelto
			char state = getState();
			movements_record[numberOfMovements] = state;
			movements_record_time[numberOfMovements] = tiempoGrabacion;
		}

		//Escribir en el fichero
		FILE* grabacio;
		grabacio = fopen(grabacioFilename, "w");

		//Grabar posicion inicial
		fprintf(grabacio, "%lf\n", grabacion_angulo_inicial);
		fprintf(grabacio, "%lf\n", grabacion_velocidad_inicial);
		fprintf(grabacio, "%lf\n", grabacion_aceleracion_inicial);
		fprintf(grabacio, "%lf\n", grabacion_friccion_inicial);

		//Grabar movimientos
		fprintf(grabacio, "%d\n", numberOfMovements + 1);
		for (int i = 0; i < numberOfMovements + 1; i++)
		{
			if (movements_record[i] != 'A' && movements_record[i] != 'Z' && movements_record[i] != 'F')
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					fprintf(grabacio, "%c%lf", movements_record[i], movements_record_time[i]);					//Si es el ultimo le quita el salto de linea
				}
				else {
					fprintf(grabacio, "%c%lf\n", movements_record[i], movements_record_time[i]);
				}
			}
			else
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					if (movements_record[i] == 'F')fprintf(grabacio, "%c%lf %d %d", movements_record[i], movements_record_time[i], HURAKAN_FRENO,V_MAXIMA-2);
					else fprintf(grabacio, "%c%lf %d %d", movements_record[i], movements_record_time[i], HURAKAN_ACELERACION,V_MAXIMA-2);					//Si es el ultimo le quita el salto de linea
				}
				else {
					if (movements_record[i] == 'F')fprintf(grabacio, "%c%lf %d %d\n", movements_record[i], movements_record_time[i], HURAKAN_FRENO,V_MAXIMA-2);
					else fprintf(grabacio, "%c%lf %d %d\n", movements_record[i], movements_record_time[i], HURAKAN_ACELERACION,V_MAXIMA-2);
				}
			}
		}
		fclose(grabacio);
	}
	if (!isGrabando && grabacio)	//Se inicia la grabacion
	{
		recienIniciado = true;
		numberOfMovements = 0; //Reiniciem els moviments
		tiempoGrabacion = 0;
		lastGrabacioInstant = instant;

		//Guardamos la posicion inicial
		grabacion_angulo_inicial = angle.x;
		grabacion_velocidad_inicial = velo_angular;
		grabacion_aceleracion_inicial = aceleracion;
		grabacion_friccion_inicial = fricc;
	}
	isGrabando = grabacio;
}

char Objeto::getState()
{
	char state = 'W';
	switch (lastEstado)
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

void Objeto::write_moves()
{
	if (isGrabando)
	{
		if (recienIniciado)
		{
			recienIniciado = false;
			lastEstado = estado;
		}
		if (lastEstado != estado)
		{
			tiempoGrabacion = instant - lastGrabacioInstant;
			lastGrabacioInstant = instant;
			char state = getState();
			movements_record[numberOfMovements] = state;
			movements_record_time[numberOfMovements] = tiempoGrabacion;
			numberOfMovements++;
			tiempoGrabacion = 0;	//Reinicia el tiempo
			lastEstado = estado;
			if (numberOfMovements == NUM_OF_MOVEMENTS)
			{
				setGrabacio(false);		//si no hay mas hueco en el array finaliza la grabación
			}
		}
		else
		{
			//tiempoGrabacion += 0.01; //Suma 10 ms
		}
	}
}