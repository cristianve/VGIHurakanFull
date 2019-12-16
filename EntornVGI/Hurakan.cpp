#include "stdafx.h"
#include "Hurakan.h"

Hurakan::Hurakan()
{
	angle_abs_brac = 0;
	cap_seient.alfa = 0;
	cap_seient.beta = 0;
	demo_on = false;
	seient.set_pes(7000);
	brac.set_fricc(12);
	seient.set_fricc(8);
	//Iniciamos el brazo
	brac.set_estat(LIBRE);
	brac.set_desp_origen(DESP_ORIGEN_BRAC_X, DESP_ORIGEN_BRAC_Y, DESP_ORIGEN_BRAC_Z);
	brac.set_angle(0, 0, 0);
	brac.set_v_angular(0);

	//Iniciamos los asientos
	seient.set_estat(LIBRE);
	seient.set_desp_origen(DESP_ORIGEN_ASIENTO_X, DESP_ORIGEN_ASIENTO_Y, DESP_ORIGEN_ASIENTO_Z);
	seient.set_angle(0, 0, 0);
	seient.set_v_angular(0);
	if (brac.get_duracio() > seient.get_duracio()) duracio_total = brac.get_duracio();
	else duracio_total = seient.get_duracio();
	//ini_movs();

	//Iniciamos el clock
	set_start();
}


void Hurakan::get_pos_seients(double& pos_x, double& pos_y, double& pos_z) {
	
	//double ang_seient = seient.get_angle_free_move()*3.14/180;
	double ang_seient = seient.get_angle().x * 3.14 / 180;
	double ang = brac.get_angle().x * 3.14 / 180;

	pos_x = POS_ASIENTO_X;
	
	pos_y = POS_ASIENTO_Y+ sin(ang)*LONGITUD_BRAC;
	pos_z = POS_ASIENTO_Z+(LONGITUD_BRAC-(cos(ang)*LONGITUD_BRAC));
	cap_seient.R = CAMP_VISIO_SEIENT;
	if (pan_alfa > 10) pan_alfa = 10;
	if (pan_alfa < -30) pan_alfa = -30;
	if (pan_beta > 90) pan_beta = 90;
	if (pan_beta < -90) pan_beta = -90;
	
	cap_seient.beta = 90*3.14/180;
	cap_seient.alfa = ang_seient+ang;





}


void Hurakan::dibuixa_hurakan(bool textu, GLint VTextu[NUM_MAX_TEXTURES]) {

	coord pos_brac_origen = brac.get_desp_origen();
	coord pos_seient_origen = seient.get_desp_origen();
	coord angle_brac = brac.get_angle();
	coord angle_seient = seient.get_angle();
	angle_abs_brac = angle_abs_brac +brac.get_angle().x;

	//Brac
	glPushMatrix();
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTEBRAC);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBRAC]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glTranslatef(-pos_brac_origen.x, pos_brac_origen.y, pos_brac_origen.z);
		glRotatef(angle_brac.x, 1, 0, 0);
		glTranslatef(pos_brac_origen.x, pos_brac_origen.y, -pos_brac_origen.z);
		glCallList(OBJECTEBRAC);
	
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTESEIENT);
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

	//Base
	glPushMatrix();
		//TEXURES
		if (textu) 
		{
			glActiveTexture(OBJECTEBASE);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEBASE]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		glCallList(OBJECTEBASE);
	glPopMatrix();

	//Floor
	glPushMatrix();
		//TEXURES
		if (textu) 
		{
			glActiveTexture(OBJECTEFLOOR);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEFLOOR]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		//glTranslatef(0, 0, -0.35);
		glCallList(OBJECTEFLOOR);
	glPopMatrix();

	//Grass
	glPushMatrix();
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTEGRASS);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEGRASS]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		//glTranslatef(0, 0, -0.35);
		glCallList(OBJECTEGRASS);
	glPopMatrix();

	//Walls
	glPushMatrix();
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTEWALLS);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEWALLS]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		//glTranslatef(0, 0, -0.35);
		glCallList(OBJECTEWALLS);
	glPopMatrix();

	//Others
	glPushMatrix();
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTEOTHERS);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTEOTHERS]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		//glTranslatef(0, 0, -0.35);
		glCallList(OBJECTEOTHERS);
	glPopMatrix();

	//Temple
	glPushMatrix();
		//TEXURES
		if (textu)
		{
			glActiveTexture(OBJECTETEMPLE);
			glBindTexture(GL_TEXTURE_2D, VTextu[OBJECTETEMPLE]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glEnable(GL_TEXTURE_2D);
		}//
		//glTranslatef(0, 0, -0.35);
		glCallList(OBJECTETEMPLE);
	glPopMatrix();

	//Skydome
	glPushMatrix();
		//TEXURES
		if (textu) 
		{
			glActiveTexture(OBJECTESKYDOME);
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


void Hurakan::move_step() 
{
	brac.set_instant(instant);
	seient.set_instant(instant);
	if (demo_on) 
	{
		brac.set_angle_moviment_lliure(brac.get_angle().x);
		brac.step();
		seient.set_angle_moviment_lliure((brac.get_angle().x) + seient.get_angle().x);
		seient.step();
	}
	else
	{
		//Modo teclado o mando
		brac.set_angle_moviment_lliure(brac.get_angle().x);
		brac.stepTeclat();
		seient.set_angle_moviment_lliure((brac.get_angle().x) + seient.get_angle().x);
		seient.stepSeientTeclat();
	}
}

void Hurakan::reset_demo()
{
	brac.reset_moves();
	seient.reset_moves();
	brac.set_angle(0, 0, 0);
	seient.set_angle(0, 0, 0);
	brac.set_v_angular(0);
	seient.set_v_angular(0);
};

void Hurakan::start_demo(char* brac_moves,char* seient_moves)
{
	set_start();
	demo_on = true;
	//inicialitzant direccio moviments
	brac.read_moves(brac_moves, instant);
	seient.read_moves(seient_moves, instant);

	dur_pausa = 0;

	brac.set_desp_origen(0.5, 0.067, 8.93);
	seient.set_desp_origen(0.5,0,2.64);
/*	brac.set_angle(0, 0, 0);
	seient.set_angle(0, 0, 0);
	brac.set_v_angular(0);
	seient.set_v_angular(0);*/
	duracio_total = brac.get_duracio();
};

//Metodos grabacion
void Hurakan::setGravacio()
{
	brac.set_gravacio_filename("grabacio_brac.txt");
	seient.set_gravacio_filename("grabacio_seients.txt");
}
void Hurakan::initGravacio()
{
	brac.setGravacio(true);
	seient.setGravacio(true);
}
void Hurakan::stopGravacio()
{
	brac.setGravacio(false);
	seient.setGravacio(false);
}