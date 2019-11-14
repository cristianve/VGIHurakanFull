#include "stdafx.h"
#include "Objeto.h"
void Objeto::step() {
	
	move_act->set_instant(instant);
	double time = (instant - lastInstant);
	if (move_act->is_libre && !move_act->is_finished() && move_act->get_type() != 3)
	{
		freeStep(time);
	}
	else
	{
		//si el moviment ha acabat i hi ha algun a la cua
		if (move_act->is_finished() && !moves.empty()) {
			
			aceleracion = move_act->get_acc();
			if (move_act->get_type() == 2) {
				aceleracion = -move_act->get_freno();
			}
			velo_angular = move_act->getMoveV();
			//s'agafa el nou moviment de la cua
			moves.pop_mov();
			if (!moves.empty()) {
				move_act = moves.top();
				move_act->set_t_ini(instant);
				//s'assigna la velocitat angular del moviment anterior al nou moviment
				move_act->setMoveV(velo_angular);
				//Es treu el moviment realitzat de la cua de moviments
				}
				move_act->set_acc(aceleracion);
				if (move_act->get_type() == 2) {
					move_act->set_freno(-aceleracion);
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
			freeStep(time);
			break;
		case ACELERAR_POSITIVO:
			if(velo_angular < MAXIMA_VELOCIDAD_ACELERACION)
				acelerar(time, true);
			else
				freeStep(time);
			break;
		case ACELERAR_NEGATIVO:
			if (velo_angular > -MAXIMA_VELOCIDAD_ACELERACION)
				acelerar(time, false);
			else
				freeStep(time);
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
		freeStep(time);
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
		freeStep(time);
	}
}

void Objeto::add_move(Move* m) {
	//actualitza duracio
	duracio += m->getMoveDur();
	//afegeix a la cua de moviments
	moves.push_mov(m);
	//actualitza l'actual i els seus paràmetres.
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
	duracio = 0;
	this->instant = instant;

	movements = fopen(filename, "r");
	if (movements != NULL)
	{
		n_llegits = fscanf(movements, "%d\n", &n_moves);
		for (int i = 0; i < n_moves; i++) {
			n_llegits = fscanf(movements, "%c", &type);
			n_llegits = fscanf(movements, "%lf\n", &time);

			Move* m = new Move;
			switch (type)
			{
			case 'W':
				m->setMove_wait(time);
				break;
			case 'F':
				m->setMove_freno(time);
				break;
			case 'A':
				m->setMove_acc(1, V_MAXIMA,time);
				break;
			case 'Z':
				m->setMove_acc(-1, V_MAXIMA, time);
				break;
			case 'L':
				m->setfreemove(time);
				break;
			default:
				break;
			}
			add_move(m);

		}
		moves.top()->set_t_ini(instant);
		fclose(movements);
	}



}

void Objeto::freeStep(double time)
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