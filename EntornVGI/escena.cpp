#include "stdafx.h"
#include "material.h"
#include "visualitzacio.h"
#include "escena.h"

// Variables de color i reflexió de materials pels objectes d'escena
CColor color_objecte;
bool reflexio_material, sw_materials[4];

// dibuixa_EscenaGL: Dibuix de l'escena amb comandes GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4], bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
	int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC, Demo_prova d1, bool demo_on)
{
	float altfar = 0;

	// Assignació de les variables de color i reflexió als valors de les variables per paràmetre
	color_objecte = col_object;
	reflexio_material = ref_mat;
	for (int i = 0; i < 4; i++) sw_materials[i] = sw_mat[i];

	if (texturID[0] != -1) SetTextureParameters(texturID[0], true, true, textur_map, false);
	if (textur) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);

	switch (objecte)
	{


		// Dibuix del Hurakan
	case HURAKAN:
		//glDisable(GL_TEXTURE_2D);
		//SeleccionaColor(ref_mat, sw_mat, col_object);
		d1.dibuixa_demo1(textur, texturID);
		
		glPushMatrix();
		glTranslatef(0, 0, -0.8);
		//sea();
		glPopMatrix();
		break;

		// Dibuix de l'objecte 3DS
	case OBJ3DS:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Dibuix de l'objecte 3DS amb textures (OBJECTE3DST) o sense textures (OBJECTE3DS)
		if (textur) glCallList(OBJECTE3DST);
		else glCallList(OBJECTE3DS);
		break;

		// Dibuix de l'objecte OBJ
	case OBJOBJ:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

	case SEIENTS:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

	case BASE:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

	case BRAZO:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

	case SKYDOME:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;
	case FLOOR:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;
	
		// Dibuix de la resta d'objectes
	default:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		//dibuixa(objecte);
		break;
	}

	// Enviar les comandes gràfiques a pantalla
	//	glFlush();
}
