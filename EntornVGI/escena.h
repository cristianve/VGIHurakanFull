//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2019 MULTIFINESTRA amb OpenGL, interf�cie MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Setembre 2018)
// escena.h : interface de escena.cpp
//
#include "Hurakan.h"

#ifndef GLSOURCE_H
#define GLSOURCE_H

/* ------------------------------------------------------------------------- */
/*                            Funcions de les escenes                        */
/* ------------------------------------------------------------------------- */
// Entorn VGI: dibuixa_EscenaGL -> Dibuix de l'escena GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4], bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
	int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC,Hurakan d1,bool demo_on);

// Entorn VGI: dibuixa -> Funci� que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(char obj);

// Entorn VGI: Truck - Objecte fet per alumnes.
void truck(bool textu,GLint VTextu[NUM_MAX_TEXTURES]);
void neumatic(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void llanta(bool textur, GLint VTextur[NUM_MAX_TEXTURES]);
void sea(void);

// Entorn VGI: Tie (Nau enemiga Star Wars) - Objecte fet per alumnes.
void tie();
void Alas();
void Motor();
void Canon();
void Cuerpo();
void Cabina();
#endif