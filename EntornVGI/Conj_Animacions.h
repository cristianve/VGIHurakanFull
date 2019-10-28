#pragma once
#include "Animacio.h"
struct QAnimacio {
	Animacio* el;
	QAnimacio* next_move;
};
class Conj_Animacio
{
private:
	QAnimacio* primer = nullptr;
	QAnimacio* ultim = nullptr;
	int n_moves = 0;
public:
	Conj_Animacio() {
	};

	void del_Conj() {
		while (ultim != nullptr) {
			pop_Animacio();
		}
	}

	void push_Animacio_back(Animacio* m) {
		QAnimacio* move = new QAnimacio;
		move->el = m;
		move->next_move = nullptr;
		if (n_moves == 0) {
			primer = move;
			ultim = move;
			++n_moves;
		}
		else {
			QAnimacio* aux_move = primer;
			primer = move;
			primer->next_move = aux_move;
			++n_moves;
		}
	};
	bool empty() {
		return n_moves == 0;
	};
	int get_n_movs() {
		return n_moves;
	};
	void pop_Animacio() {
		if (ultim != nullptr) {
			QAnimacio* it_move = primer;
			int i = 0;
			for (i = 0; i < n_moves - 2; ++i) {
				it_move = it_move->next_move;
			}
			it_move->next_move = nullptr;
			delete(ultim);
			--n_moves;
			ultim = it_move;
		}
	}

	QAnimacio* get_iterator() {
		if (primer != nullptr) {
			return primer;
		}
		else {
			return nullptr;
		}
	}

	Animacio* top() {
		if (ultim != nullptr) {
			return ultim->el;
		}
		else {
			return nullptr;
		}
	}
	Animacio* bottom() {
		if (primer != nullptr) {
			return primer->el;
		}
		else {
			return nullptr;
		}
	}

};

