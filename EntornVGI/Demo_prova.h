#pragma once
#include "visualitzacio.h"
#include "Animacio.h"
#include "stdafx.h"
#include "constants.h"

class Demo_prova
{

private:
	float t = 0;
	
	bool textura = false;

	coord angle_brac;
	coord pos_brac_origen;

	coord angle_seient;
	coord pos_seient_origen;

	Animacio rot_brac;

	

public:
	
	int mode = 0;
	double interval = 10; // en ms (no cal canviar, resolucio de l'animació(afecta a la velocitat))
	double duracion_total; // en sec
	double instant = 0;
	
	
	Demo_prova() {
	
		/*
		glGenTextures(1, &textura_terra);
		glBindTexture(GL_TEXTURE_2D, textura_terra);
		textura_terra = loadIMA_ILUT("img/Flat_earth.jpg");
		textura_terra = SOIL_load_OGL_texture("Flat_earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y);

		glGenTextures(1, &textura_sol);
		glBindTexture(GL_TEXTURE_2D, textura_sol);
		textura_sol = SOIL_load_OGL_texture("Flat_sol.jpg", 0, textura_sol, 0);

		glGenTextures(1, &textura_lluna);
		glBindTexture(GL_TEXTURE_2D, textura_lluna);
		textura_lluna = SOIL_load_OGL_texture("Flat_lluna.jpg", 0, textura_lluna, 0);
		*/
		textura = true;
		t = 0;
		
	};
	Demo_prova(float t, float angle_1) {
		/*
		glGenTextures(1, &textura_terra);
		glBindTexture(GL_TEXTURE_2D, textura_terra);
		textura_terra = SOIL_load_OGL_texture("res/Flat_earth.jpg", 0, textura_terra, 0);

		glGenTextures(1, &textura_sol);
		glBindTexture(GL_TEXTURE_2D, textura_sol);
		textura_sol = SOIL_load_OGL_texture("res/Flat_sol.jpg", 0, textura_sol, 0);

		glGenTextures(1, &textura_lluna);
		glBindTexture(GL_TEXTURE_2D, textura_lluna);
		textura_lluna = SOIL_load_OGL_texture("res/Flat_lluna.jpg", 0, textura_lluna, 0);
		*/
		this->t = t;
		
	};

	

	void start_demo() {
		//inicialitzant direccio moviments
		pos_brac_origen.x = 0.5;
		pos_brac_origen.y = 0;
		pos_brac_origen.z = 8.94;
		angle_brac.x = 90;
		pos_seient_origen.x = 0.5;
		pos_seient_origen.y = 0;
		pos_seient_origen.z = 3;
		angle_seient.x = 50;
		ini_movs();
		
		
	}

	void load_textures(GLuint tex_earth, GLuint tex_sun, GLuint tex_moon);
	void dibuixa_demo1();
	void ini_movs();

	void move_step();
	void augmenta_angle();
	void move_step_v0();

};
