#pragma once
#include "MoveQueue.h"



class Animacio
{
private:
	// parts mov lineal
	coord object_pos;
	v object_v;
	v v_angular;
	acc object_acc;
	dir object_dir;
	coord angle_obj;
	coord angle_ini;

	double object_id= 0;

	double instant = 0;
	double duracio = 0;

	Move* move_act = nullptr;

	MoveQueue moviments;

public:

	Animacio() {

	};
	void del_Animacio() {
		moviments.deleteQueue();
		delete(move_act);
	}
	
	void set_instant(double instant) {
		this->instant = instant;
	};

	coord get_angle() {
		return angle_obj;
	}

	double get_duracio() {
		return duracio;
	};

	coord get_pos() {
		return object_pos;
	};

	void set_pos(coord pos) {
		this->object_pos = pos;
	};

	void step_animacio() {
		move_act->set_instant(instant);
		if (move_act->is_finished() && !moviments.empty()) {
			if (move_act->get_type()) {
				v_angular = move_act->getMoveV();
				move_act = moviments.top();
				move_act->setMoveV(v_angular);
			}
			else {
				object_v = move_act->getMoveV();
				move_act = moviments.top();
				move_act->setMoveV(object_v);
			}
			moviments.pop_mov();
		}
		else {
			if (move_act->get_type()) {
				move_act->move_step_rot(angle_obj);
			}
			else {
				move_act->move_step_lin(object_pos);
			}
		}
	};

	void add_move(Move *m) {
		duracio += m->getMoveDur();
		moviments.push_mov(m);
		move_act = moviments.top();
		object_dir = move_act->getMoveDir();
		object_acc = move_act->getMoveAcc();
		object_v = move_act->getMoveV();
		if (move_act->get_type()) {
			v_angular = move_act->getMoveV();
		}
	};

	
};

