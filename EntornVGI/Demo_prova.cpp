#include "stdafx.h"
#include "Demo_prova.h"

void Demo_prova::dibuixa_demo1() {
	angle_brac = rot_brac.get_angle();

	glPushMatrix();
		glTranslatef(-pos_brac_origen.x, pos_brac_origen.y, pos_brac_origen.z);
		glRotatef(angle_brac.x, 1, 0, 0);
		glTranslatef(pos_brac_origen.x, pos_brac_origen.y, -pos_brac_origen.z);
		glCallList(OBJECTEBRAC);

		glPushMatrix();
			glTranslatef(-pos_seient_origen.x, pos_seient_origen.y, pos_seient_origen.z);
			glRotatef(angle_seient.x, 1, 0, 0);
			glTranslatef(pos_seient_origen.x, pos_seient_origen.y, -pos_seient_origen.z);
			glCallList(OBJECTESEIENT);
		glPopMatrix();

	glPopMatrix();



	glPushMatrix();
		glCallList(OBJECTEBASE);
	glPopMatrix();
}



void Demo_prova::move_step() {
	rot_brac.set_instant(instant);
	//rot_brac.step_animacio();
}

void Demo_prova::ini_movs() {
	Move* rot_brac_acc = new Move;
	Move* rot_brac_freno = new Move;

	rot_brac_acc->setMove_Rotacional(1, 0, 0, 1, 0, 0, 3, 0, 0, 10);
	rot_brac_acc->set_t_ini(0);
	
	rot_brac_freno->setMove_Rotacional(1, 0, 0, -1, 0, 0, 3, 0, 0, 10);
	rot_brac_freno->set_t_ini(rot_brac_acc->get_t_final());

	rot_brac.add_move(rot_brac_acc);
	rot_brac.add_move(rot_brac_freno);
	duracion_total = rot_brac.get_duracio()/3.34;

	
}
