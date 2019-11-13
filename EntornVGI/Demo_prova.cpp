#include "stdafx.h"
#include "Demo_prova.h"

void Demo_prova::dibuixa_demo1(bool textu, GLint VTextu[NUM_MAX_TEXTURES]) {

	coord pos_brac_origen = brac.get_desp_origen();
	coord pos_seient_origen = seient.get_desp_origen();
	coord angle_brac = brac.get_angle();
	coord angle_seient = seient.get_angle();

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBRAC]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	//
	glEnable(GL_TEXTURE_2D);
	


	glTranslatef(-pos_brac_origen.x, pos_brac_origen.y, pos_brac_origen.z);
	glRotatef(angle_brac.x, 1, 0, 0);
	glTranslatef(pos_brac_origen.x, pos_brac_origen.y, -pos_brac_origen.z);
	glCallList(OBJECTEBRAC);

	glPushMatrix();
	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTESEIENT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//

	glTranslatef(-pos_seient_origen.x, pos_seient_origen.y, pos_seient_origen.z);
	glRotatef(angle_seient.x, 1, 0, 0);
	glTranslatef(pos_seient_origen.x, pos_seient_origen.y, -pos_seient_origen.z);
	glCallList(OBJECTESEIENT);
	glPopMatrix();

	glPopMatrix();



	glPushMatrix();
	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBASE]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glCallList(OBJECTEBASE);
	glPopMatrix();
	glPushMatrix();
	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEFLOOR]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//

	glCallList(OBJECTEFLOOR);
	glPopMatrix();
	glPushMatrix();

	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTESKYDOME]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glCallList(OBJECTESKYDOME);
	glPopMatrix();
}
void Demo_prova::dibuixa_inicial(bool textu, GLint VTextu[NUM_MAX_TEXTURES]) {
	coord pos_brac_origen = brac.get_desp_origen();
	coord pos_seient_origen = seient.get_desp_origen();
	
	glPushMatrix();
	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBRAC]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glTranslatef(pos_brac_origen.x, pos_brac_origen.y, pos_brac_origen.z);
	glRotatef(0, 1, 0, 0);
	glTranslatef(pos_brac_origen.x, pos_brac_origen.y, -pos_brac_origen.z);
	glCallList(OBJECTEBRAC);

	glPushMatrix();
	//TEXURES
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTESEIENT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glTranslatef(pos_seient_origen.x, pos_seient_origen.y, pos_seient_origen.z);
	glRotatef(0, 1, 0, 0);
	glTranslatef(pos_seient_origen.x, pos_seient_origen.y, -pos_seient_origen.z);
	glCallList(OBJECTESEIENT);
	glPopMatrix();

	glPopMatrix();



	glPushMatrix();

	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBASE]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glCallList(OBJECTEBASE);
	glPopMatrix();

	glPushMatrix();
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEFLOOR]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
	}//
	glCallList(OBJECTEFLOOR);
	glPopMatrix();

	glPushMatrix();
	//TEXURES
	if (textu) {
		glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTESKYDOME]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glEnable(GL_TEXTURE_2D);
		
	}
	//
	glCallList(OBJECTESKYDOME);
	glPopMatrix();
}


void Demo_prova::move_step() {
	
	brac.set_instant(instant);
	seient.set_instant(instant);
	if (demo_on) {
		brac.set_angle_abs(brac.get_angle().x);
		brac.step();
		seient.set_angle_abs(brac.get_angle().x + seient.get_angle().x);
		seient.step();
	}
	
	
	
}



