#pragma once
#include "Move.h"

struct QMove {
	int id;
	Move* el;
	QMove* next_move;
};
class MoveQueue
{
private:
	QMove* primer = nullptr;
	QMove* ultim = nullptr;
	int n_moves = 0;
public:
	MoveQueue() {
	};
	
	void deleteQueue() {
		while (ultim != nullptr) {
			pop_mov();
		}
	}
	
	void push_mov(Move* m) {
		QMove* move = new QMove;
		move->id = n_moves;
		move->el = m;
		move->next_move = nullptr;
		if (n_moves==0) {
			primer = move;
			ultim = move;
			++n_moves;
		}
		else {
			QMove* aux_move = primer;
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
	void pop_mov() {
		if (ultim!=nullptr){
			QMove* it_move = primer;
			int i = 0;
			for (i = 0; i < n_moves-2; ++i) {
				it_move = it_move->next_move;
			}
			it_move->next_move = nullptr;
			delete(ultim);
			--n_moves;
			ultim = it_move;
		}
	}

	QMove* get_iterator() {
		if (primer != nullptr) {
			return primer;
		}
		else {
			return nullptr;
		} 
	}

	Move* top() {
		if (ultim != nullptr) {
			return ultim->el;
		}
		else {
			return nullptr;
		}
	}
	Move* bottom() {
		if (primer != nullptr) {
			return primer->el;
		}
		else {
			return nullptr;
		}
	}

};

