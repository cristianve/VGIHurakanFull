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
	if (set_velo) {
		move_act->setMoveV(velo_angular);
		set_velo = false;
	}
	move_act->set_instant(instant);
	double time = (instant - lastInstant);
	if (move_act->is_libre && !move_act->is_finished() && move_act->get_type() != 3)
	{
		if (move_act->getMoveDir() == 1)freeStep_f(time);
		else if (move_act->getMoveDir() == -1)freeStep_b(time);
		else freeStep_f(time);
	}
	else
	{
		//si el moviment ha acabat i hi ha algun a la cua
		if (move_act->is_finished() && !moves.empty()) {
			
			velo_angular = move_act->getMoveV();
			d = move_act->getMoveDir();
			//s'agafa el nou moviment de la cua
			moves.pop_mov();
			if (!moves.empty()) {
				move_act = moves.top();
				move_act->set_t_ini(instant);
				//s'assigna la velocitat angular del moviment anterior al nou moviment
				move_act->setMoveV(velo_angular);
				if (move_act->get_type() != 0 && move_act->get_type() != 1) move_act->setMoveDir(d);
				//Es treu el moviment realitzat de la cua de moviments
				
			}
		}
		//si el moviment no ha acabat pero el instant actual es major al seu temps d'inici
		else if (!move_act->is_finished() && move_act->get_t_ini() <= instant) {
				//avancem movimen
			move_act->move_step_rot(angle,time);
		}
	}
}

void Objeto::stepTeclado()
{
	double time = (instant - lastInstant);
	switch (estado)
	{
		case FRENAR:
			frenar(time);
			break;
		case LIBRE:
			freeStep_f(time);
			break;
		case ACELERAR_POSITIVO:
			if(velo_angular < MAXIMA_VELOCIDAD_ACELERACION)
				acelerar(time, true);
			else
				freeStep_f(time);
			break;
		case ACELERAR_NEGATIVO:
			if (velo_angular > -MAXIMA_VELOCIDAD_ACELERACION)
				acelerar(time, false);
			else
				freeStep_f(time);
			break;
		case CLAVAR_BRAZO:
			if ((velo_angular > MAXIMA_VELOCIDAD_CLAVAR) || (velo_angular < (MAXIMA_VELOCIDAD_CLAVAR)*(-1)) || aceleracion > MAXIMA_ACELERACION_CLAVAR || aceleracion < (MAXIMA_ACELERACION_CLAVAR)*(-1)) //Debe de estar en el rango de aceleracion y velocidad para clavar
			{
				frenar(time);
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
			frenar(time);
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
		acelerar(time, true);
		break;
	case GIRAR_NEGATIVO:
		isTambaleoTimeSet = false;
		acelerar(time, false);
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
	if(tambaleoInstant <= 2)
	{ 
		acelerar(time, true);
	}
	else
	{
		freeStep_f(time);
	}
}

void Objeto::add_move(Move* m) {
	//actualitza duracio
	duracio += m->getMoveDur();
	//afegeix a la cua de moviments
	moves.push_mov(m);
	//actualitza l'actual i els seus par�metres.
	move_act = moves.top();
	direc = move_act->getMoveDir();
	accel_dir = move_act->getMoveAcc();
	velo = move_act->getMoveV();
	velo_angular = move_act->getMoveV();
}

void Objeto::read_moves(char* filename,double instant) {
	FILE* movements;
	int n_moves = 0;
	char type = 'X';
	char aux = 'X';
	int n_llegits;
	double time = 0.0;
	double acc = 0.0;
	duracio = 0;
	this->instant = instant;

	movements = fopen(filename, "r");
	if (movements != NULL)
	{
		n_llegits = fscanf(movements, "%d\n", &n_moves);
		for (int i = 0; i < n_moves; i++) {
			n_llegits = fscanf(movements, "%c", &type);
			n_llegits = fscanf(movements, "%lf", &time);

			Move* m = new Move;
			switch (type)
			{
			case 'W':
				m->setMove_wait(time);
				break;
			case 'F':
				n_llegits = fscanf(movements, " %lf", &acc);
				m->setMove_freno(moves.top()->getMoveDir(),time);
				m->set_freno(acc);
				break;
			case 'A':
				n_llegits = fscanf(movements, " %lf", &acc);
				m->setMove_acc(1, V_MAXIMA,time);
				m->set_acc(acc);
				break;
			case 'Z':
				n_llegits = fscanf(movements, " %lf", &acc);
				m->setMove_acc(-1, V_MAXIMA, time);
				m->set_acc(acc);
				break;
			case 'L':
				m->setfreemove(time);
				break;
			default:
				break;
			}
			fscanf(movements, "\n");
			add_move(m);

		}
		moves.top()->set_t_ini(instant);
		fclose(movements);
	}



}

void Objeto::freeStep_f(double time)
{
	bool izq = false;
	float actualAngle = angle_abs;

	if (actualAngle > 180)
	{
		actualAngle = actualAngle - 180;
		izq = true;
	}
	

	float rozamiento = -0.5 * velo_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = peso * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	aceleracion = (fuerzaTotal / (peso*0.15))*10;


	velo_angular += (aceleracion)*time;
	
	angle.x = angle.x + (velo_angular * time);

	if (angle.x >= 360) {
		angle.x = angle.x - 360;
	}
}

void Objeto::acelerar(double time, bool isPositivo)
{
	if (velo_angular > V_MAXIMA) {
		velo_angular = V_MAXIMA;
	}
	else {
		aceleracion = HURAKAN_ACELERACION;
		if (!isPositivo)
			aceleracion *= (-1);
		velo_angular += aceleracion * time;
	}
	angle.x = angle.x + (velo_angular * time);
}

void Objeto::frenar(double time)
{
	if (velo_angular > 0)
	{
		velo_angular -= HURAKAN_FRENO * time;
		if (velo_angular < 0) velo_angular = 0;
	}
	else
	{
		velo_angular += HURAKAN_FRENO * time;
		if (velo_angular > 0) velo_angular = 0;
	}
	angle.x = angle.x + (velo_angular * time);
}
void Objeto::freeStep_b(double time)
{
	bool izq = true;
	float actualAngle = angle_abs;

	if (actualAngle < -180)
	{
		actualAngle = actualAngle + 180;
		izq = false;
	}


	float rozamiento = -0.5 * velo_angular;
	actualAngle = actualAngle * 3.14 / 180;
	float fuerzaPorPeso = peso * sin(actualAngle);

	if (!izq) fuerzaPorPeso *= -1;


	float fuerzaTotal = fuerzaPorPeso + rozamiento;
	aceleracion = (fuerzaTotal / (peso * 0.15)) * 10;


	velo_angular += (aceleracion)*time;

	angle.x = angle.x - (velo_angular * time);

	if (angle.x <= -360) {
		angle.x = angle.x + 360;
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
			movements[numberOfMovements] = state;
			movements_time[numberOfMovements] = tiempoGrabacion;
		}

		//Escribir en el fichero
		FILE* grabacio;
		grabacio = fopen(grabacioFilename, "w");
		fprintf(grabacio, "%d\n", numberOfMovements + 1);
		for (int i = 0; i < numberOfMovements + 1; i++)
		{
			if (movements[i] != 'A' && movements[i] != 'Z')
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					fprintf(grabacio, "%c%lf", movements[i], movements_time[i]);					//Si es el ultimo le quita el salto de linea
				}
				else {
					fprintf(grabacio, "%c%lf\n", movements[i], movements_time[i]);
				}
			}
			else
			{
				if (i + 1 == NUM_OF_MOVEMENTS) {
					fprintf(grabacio, "%c%lf %d", movements[i], movements_time[i], HURAKAN_ACELERACION);					//Si es el ultimo le quita el salto de linea
				}
				else {
					fprintf(grabacio, "%c%lf %d\n", movements[i], movements_time[i], HURAKAN_ACELERACION);
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
			movements[numberOfMovements] = state;
			movements_time[numberOfMovements] = tiempoGrabacion;
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