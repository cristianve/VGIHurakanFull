#include "stdafx.h"
#include "Demo_prova.h"

void Demo_prova::dibuixa_demo1() {
	coord pos_brac_origen = brac.get_desp_origen();
	coord pos_seient_origen = seient.get_desp_origen();
	coord angle_brac = brac.get_angle();
	coord angle_seient = seient.get_angle();
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
void Demo_prova::dibuixa_inicial() {
	coord pos_brac_origen = brac.get_desp_origen();
	coord pos_seient_origen = seient.get_desp_origen();
	
	glPushMatrix();
	glTranslatef(pos_brac_origen.x, pos_brac_origen.y, pos_brac_origen.z);
	glRotatef(0, 1, 0, 0);
	glTranslatef(pos_brac_origen.x, pos_brac_origen.y, -pos_brac_origen.z);
	glCallList(OBJECTEBRAC);

	glPushMatrix();
	glTranslatef(pos_seient_origen.x, pos_seient_origen.y, pos_seient_origen.z);
	glRotatef(0, 1, 0, 0);
	glTranslatef(pos_seient_origen.x, pos_seient_origen.y, -pos_seient_origen.z);
	glCallList(OBJECTESEIENT);
	glPopMatrix();

	glPopMatrix();



	glPushMatrix();
	glCallList(OBJECTEBASE);
	glPopMatrix();
}


void Demo_prova::move_step() {
	
	brac.set_instant(instant);
	brac.step();
	
	seient.set_instant(instant);
	seient.step();
	
}



