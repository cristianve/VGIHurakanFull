#include "stdafx.h"
#include "Objeto.h"
void Objeto::step() {
	int d;
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
				if (move_act->get_type() == 4) move_act->setMoveDir(d);
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