#include "stdafx.h"
#include "Objeto.h"
void Objeto::step() {
	//avança el temps de l'animació
	move_act->set_instant(instant);
	//si el moviment ha acabat i hi ha algun a la cua
	if (move_act->is_finished() && !moves.empty()) {
		if (move_act->get_type()) {
			velo_angular = move_act->getMoveV();
			//s'agafa el nou moviment de la cua
			move_act = moves.top();
			//s'assigna la velocitat angular del moviment anterior al nou moviment
			move_act->setMoveV(velo_angular);
		}
		else {
			velo = move_act->getMoveV();
			//S'agafa el nou moviment de la cua
			move_act = moves.top();
			//s'assigna la velocitat angular del moviment anterior al nou moviment
			move_act->setMoveV(velo);
		}
		//Es treu el moviment realitzat de la cua de moviments
		moves.pop_mov();
	}
	//si el moviment no ha acabat pero el instant actual es major al seu temps d'inici
	else if (move_act->get_t_ini() <= instant) {
		if (move_act->get_type()) {
			//avancem moviment
			move_act->move_step_rot(angle);
		}
		else {
			//avancem moviment
			move_act->move_step_lin(pos);
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
	if (move_act->get_type()) {
		velo_angular = move_act->getMoveV();
	}
}

void Objeto::read_moves(char* filename) {
	FILE* fp;
	char ch[1000];
	int n_moves = 0;
	int i = 0;
	int tipus = 0;

	int dirx, diry, dirz, accx, accy, accz;
	dirx = 0;
	diry = 0;
	dirz = 0;
	accx = 0;
	accy = 0;
	accz = 0;
	double vmaxx, vmaxy, vmaxz, dur;
	vmaxx = 0;
	vmaxy = 0;
	vmaxz = 0;
	dur = 0;
	double acc, freno;
	acc = freno = 0;
	int t_ini = 0;

	fp = fopen(filename, "r");
	if (fp != NULL) {
		fscanf(fp, "%d\n",&n_moves);
		while (i < n_moves) {
			Move* m = new Move;
			dirx = 0;
			diry = 0;
			dirz = 0;
			accx = 0;
			accy = 0;
			accz = 0;
			vmaxx = 0;
			vmaxy = 0;
			vmaxz = 0;
			dur = 0;
			fscanf(fp, "%d ", &tipus);
			fscanf(fp, "%d ", &t_ini);
			fscanf(fp, "%d ", &dirx);
			fscanf(fp, "%d ", &diry);
			fscanf(fp, "%d ", &dirz);
			fscanf(fp, "%d ", &accx);
			fscanf(fp, "%d ", &accy);
			fscanf(fp, "%d ", &accz);
			fscanf(fp, "%lf ", &vmaxx);
			fscanf(fp, "%lf ", &vmaxy);
			fscanf(fp, "%lf ", &vmaxz);
			fscanf(fp, "%lf ", &dur);
			fscanf(fp, "%lf ", &acc);
			fscanf(fp, "%lf", &freno);
			if (tipus) {
				m->setMove_Rotacional(dirx, diry, dirz, accx, accy, accz, vmaxx, vmaxy, vmaxz, dur);
			}
			else {
				m->setMove_Lineal(dirx, diry, dirz, accx, accy, accz, vmaxx, vmaxy, vmaxz, dur);
			}
			if (acc != 0) {
				m->set_acc(acc);
			}
			if (freno != 0) {
				m->set_freno(freno);
			}
			if (t_ini != -1) {
				m->set_t_ini(t_ini);
			}
			else {
				m->set_t_ini(moves.bottom()->get_t_final());
			}
			add_move(m);
			i++;
		}
		fclose(fp);
	}
	
}