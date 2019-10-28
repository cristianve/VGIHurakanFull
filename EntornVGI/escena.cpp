//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2019 MULTIFINESTRA amb OpenGL, interf�cie MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Setembre 2019)
// escena.cpp : Aqui es on ha d'anar el codi de les funcions que 
//              dibuixin les escenes.
//
//    Versi� 2.0:	- Objectes Cub, Esfera, Tetera (primitives libreria GLUT) i Truck (cami� americ� fet per alumnes)
//
//	  Versi� 2.2:	- Objectes Cub, Esfera, Tetera definides en fitxer font glut_geometry amb altres primitives GLUT
//
//	  Versi� 2.5:	- Objectes cubRGB i Tie (nau Star Wars fet per alumnes)
//

#include "stdafx.h"
#include "material.h"
#include "visualitzacio.h"
#include "escena.h"

// Variables de color i reflexi� de materials pels objectes d'escena
CColor color_objecte;
bool reflexio_material, sw_materials[4];

// dibuixa_EscenaGL: Dibuix de l'escena amb comandes GL
void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4], bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
			int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC,Demo_prova d1,bool demo_on)
{
	float altfar = 0;

// Assignaci� de les variables de color i reflexi� als valors de les variables per par�metre
	color_objecte = col_object;
	reflexio_material = ref_mat;
	for (int i = 0; i < 4;i++) sw_materials[i] = sw_mat[i];

	if (texturID[0] != -1) SetTextureParameters(texturID[0], true, true, textur_map, false);
	if (textur) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);

	switch (objecte)
	{

// Cub RGB
	case CUB_RGB:
		//	glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		  glTranslatef(2.5f, 2.5f, 2.5f);
		  glScalef(5.0f, 5.0f, 5.0f);
		  glutSolidCubeRGB(1.0);
		glPopMatrix();
		break;

// Dibuix de l'objecte TIE (Nau enemiga Star Wars)
	case TIE:
		tie();
		break;

// Dibuix del Truck
	case TRUCK:
		glDisable(GL_TEXTURE_2D);
		truck(textur, texturID);
		sea();
		break;

// Dibuix del Hurakan
	case HURAKAN:
		//glDisable(GL_TEXTURE_2D);
		//SeleccionaColor(ref_mat, sw_mat, col_object);
		if (demo_on) d1.dibuixa_demo1();
		else {
			if (textur) glCallList(OBJECTEOBJT);
			else glCallList(OBJECTEOBJ);
			sea();
		}
		break;
		

	case PERSONA:
		//glDisable(GL_TEXTURE_2D);
		//SeleccionaColor(ref_mat, sw_mat, col_object);
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;


// Dibuix de l'objecte 3DS
	case OBJ3DS:
		// Definici� propietats de reflexi� (emissi�, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Dibuix de l'objecte 3DS amb textures (OBJECTE3DST) o sense textures (OBJECTE3DS)
		if (textur) glCallList(OBJECTE3DST);
		else glCallList(OBJECTE3DS);
		break;

// Dibuix de l'objecte OBJ
	case OBJOBJ:
		// Definici� propietats de reflexi� (emissi�, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures (OBJECTEOBJT) o sense textures (OBJECTEOBJ)
		if (textur) glCallList(OBJECTEOBJT);
		else glCallList(OBJECTEOBJ);
		break;

// Corba Bezier
	case C_BEZIER:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	glColor3f(1.0, 0.0, 0.0);
			for (int i = 0; i < nptsU; i++)
			{	glPushMatrix();
				  glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				  glutSolidSphere(5.0, 20, 20);
				glPopMatrix();
			}
		}
		SeleccionaColor(ref_mat, sw_mat, col_object);
		//draw_Bezier_Curve(PC_u, nptsU, PAS_BEZIER,false);
		draw_Bezier_Curve(PC_u, nptsU, pasCS, false);
		break;

// Corba Lemniscata
	case C_LEMNISCATA:
		//glColor3f(1.0,1.0,1.0);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		//draw_Lemniscata(800, 10.0);
		draw_Lemniscata(800, pasCS*20.0);
		break;

// Corba Spline
	case C_BSPLINE:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	glColor3f(1.0, 0.0, 0.0);
			for (int i = 0; i < nptsU; i++)
			{	glPushMatrix();
				  glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				  glutSolidSphere(5.0, 20, 20);
				glPopMatrix();
			}
		}
		SeleccionaColor(ref_mat, sw_mat, col_object);
		//draw_BSpline_Curve(PC_u, nptsU, PAS_BSPLINE);
		draw_BSpline_Curve(PC_u, nptsU, pasCS);
		break;

// Dibuix de la resta d'objectes
	default:
		// Definici� propietats de reflexi� (emissi�, ambient, difusa, especular) del material.
		//SeleccionaMaterialiColor(MAT_CAP, ref_mat, sw_mat, col_object);
		SeleccionaColor(ref_mat, sw_mat, col_object);
		dibuixa(objecte);
		break;
	}

// Enviar les comandes gr�fiques a pantalla
//	glFlush();
}


// dibuixa: Funci� que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(char obj)
{
	switch(obj)
	{

// Cub
	case CUB:
//		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		  glScalef(5.0f,5.0f,5.0f);
		  glutSolidCube(1.0);
		glPopMatrix();
		break;

// Esfera
	case ESFERA:
//		glColor3f(1.0,1.0,1.0);	
		glPushMatrix();
		  glScalef(5.0f,5.0f,5.0f);
		  gluSphere(1.0, 40, 40);
		glPopMatrix();
		break;

// Tetera
	case TETERA:
//		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		  //glScalef(5.0f,5.0f,5.0f);
		  glutSolidTeapot(1.0);
		glPopMatrix();
		break;
	}
}


// OBJECTE Truck amb imatges textura si s�n actives
// Truck: Par�metres:
//    - textu: Flag de textures activades o no.
//    - VTextu: Vector d'imatges textura.
void truck(bool textu,GLint VTextu[NUM_MAX_TEXTURES])
{

glPushMatrix();
  glTranslatef(0.0f,0.0f,20.0f);

// MOTOR
// Textura carrosseria
	if (textu)  
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;

			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_TEXTURE_2D);
		}

// Color carrosseria sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f); 

	glPushMatrix();		
		glScalef(2.0f, 4.4f, 2.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// CABINA
// Cabina inferior
	glPushMatrix();
		glTranslatef(0.0f,32.0f,0.0f);
		glScalef(4.0f, 2.0f, 2.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Cabina superior
	glPushMatrix();
		glTranslatef(0.0f,32.0f,15.0f);
		glScalef(2.0f, 2.0f, 1.0f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Cabina trasera
	glPushMatrix();
		glTranslatef(0.0f,49.5f,12.5f);
		glScalef(4.0f, 1.5f, 4.5f);
		glutSolidCube(10.0f);
	glPopMatrix();

// CANTONADES
// Cantonada esquerra
	glPushMatrix() ;
	  glTranslatef(10.0f,22.0f,-10.0f);

  	  glBegin(GL_QUADS);  // Cara de 4 costats
	 	glNormal3f(200.0f, 200.0f, 0.0f);
		glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		glVertex3f(10.0f , 0.0f , 20.0f ); //P2
		glVertex3f(0.0f , -10.0f ,20.0f ); //P3
		glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle d'abaix
		glNormal3f(0.0f, 0.0f, -200.0f);
		glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
		glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
		glVertex3f(0.0f , -10.0f , 0.0f ); // P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle de dalt
		glNormal3f(0.0f, 0.0f, 200.0f);
		glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
	    glVertex3f(0.0f , -10.0f , 20.0f ); // P3
		glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
	  glEnd();
	glPopMatrix();

// Cantonada dreta
	glPushMatrix() ;
	  glTranslatef(-10.0f,22.0f,-10.0f);
	  glRotatef(-90.0f,0.0f,0.0f,1.0f);

  	  glBegin(GL_QUADS);  // Cara de 4 costats
	 	glNormal3f(200.0f, 200.0f, 0.0f);
		glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		glVertex3f(10.0f , 0.0f , 20.0f ); //P2
		glVertex3f(0.0f , -10.0f ,20.0f ); //P3
		glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle d'abaix
		glNormal3f(0.0f, 0.0f, -200.0f);
		glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
		glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
		glVertex3f(0.0f , -10.0f , 0.0f ); // P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle de dalt
		glNormal3f(0.0f, 0.0f, 200.0f);
		glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
	    glVertex3f(0.0f , -10.0f , 20.0f ); // P3
		glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
	  glEnd();
	glPopMatrix();


// PROTECCIONS DE RODA
// Textura Proteccions de roda (metall)
//	if (textu)
//		{				
//			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
//			glEnable(GL_TEXTURE_2D) ;
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			}

// Color proteccions roda sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Protecci� de roda esquerra
	glPushMatrix();		
		glTranslatef(10.0f, 0.0f, 3.0f);
		glBegin(GL_QUADS);
		//  glNormal3f(200.0f, 200.0f, 0.0f);
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(0.0f , 5.0f , 0.0f );
			glVertex3f(0.0f , -5.0f , 0.0f );

			glVertex3f(7.0f , -12.0f , -8.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(0.0f , -5.0f , 0.0f );
			glVertex3f(0.0f , -12.0f , -8.0f );

			glVertex3f(0.0f , 15.0f , -10.0f );
			glVertex3f(0.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , 15.0f , -10.0f );

			glVertex3f(7.5f , -5.0f , -3.0f );
			glVertex3f(7.5f , 5.0f , -3.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
				
			glVertex3f(7.5f , -12.0f , -8.0f );
			glVertex3f(7.5f , -5.0f , -3.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(7.0f , -12.0f , -8.0f );
				
			glVertex3f(7.0f , 15.0f , -10.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.5f , 5.0f , -3.0f );
			glVertex3f(7.5f , 15.0f , -10.0f );

		glEnd();
	glPopMatrix();

// Protecci� de roda dreta
	glPushMatrix();		
		glTranslatef(-16.0f, 0.0f, 3.0f);
		glBegin(GL_QUADS);
		//  glNormal3f(200.0f, 200.0f, 0.0f);
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(0.0f , 5.0f , 0.0f );
			glVertex3f(0.0f , -5.0f , 0.0f );

			glVertex3f(7.0f , -12.0f , -8.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(0.0f , -5.0f , 0.0f );
			glVertex3f(0.0f , -12.0f , -8.0f );

			glVertex3f(0.0f , 15.0f , -10.0f );
			glVertex3f(0.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , 15.0f , -10.0f );

			glVertex3f(7.5f , -5.0f , -3.0f );
			glVertex3f(7.5f , 5.0f , -3.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
				
			glVertex3f(7.5f , -12.0f , -8.0f );
			glVertex3f(7.5f , -5.0f , -3.0f );
			glVertex3f(7.0f , -5.0f , 0.0f );
			glVertex3f(7.0f , -12.0f , -8.0f );
				
			glVertex3f(7.0f , 15.0f , -10.0f );
			glVertex3f(7.0f , 5.0f , 0.0f );
			glVertex3f(7.5f , 5.0f , -3.0f );
			glVertex3f(7.5f , 15.0f , -10.0f );
		glEnd();
	glPopMatrix();

// Textura Reixeta Motor
	if (textu)  
		{	GLfloat sPlane[4] = { 0.05f, 0.25f, 0.00f, 0.00f };
		  	GLfloat tPlane[4] = { 0.00f, 0.25f, 0.05f, 0.00f };
// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[3]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color reixeta motor sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);	

// Reixeta Motor
	glPushMatrix();
		glTranslatef(0.0f,-22.0f,0.0f);
		glBegin(GL_QUADS);
		    glNormal3f(0.0f,-1.0f,0.0f);	
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(-10.0f , -2.0f , -10.0f );
			glTexCoord2f(1.0f,0.00f);
			glVertex3f(10.0f , -2.0f , -10.0f );
			glTexCoord2f(1.0f,1.0);
			glVertex3f(10.0f , -2.0f , 10.0f );
			glTexCoord2f(0.0f,1.0f);	
			glVertex3f(-10.0f , -2.0f , 10.0f );
		glEnd();
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Color eix sense textura
	glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Davanter
	glPushMatrix();
	  glTranslatef(0.0f,0.0f,-10.0f);
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
  	  glTranslatef(0.0f,0.0f,-13.0f);
	  gluCylinder(2.0,2.0,26.0f,6,1);
	glPopMatrix();
	
// Roda Dreta Davantera
	glPushMatrix();
		glTranslatef(-10.0f,0.0f,-10.0f);
		neumatic(textu,VTextu);
	glPopMatrix();

// Roda Esquerra Davantera
	glPushMatrix();
		glTranslatef(17.0f,0.0f,-10.0f);
		neumatic(textu,VTextu);
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// FARS	
// Color fars sense textura
	glColor4f(1.0f,0.0f,0.0f,0.5f);

// Far Esquerra	
	glPushMatrix();
		glTranslatef(-13.5f,-17.0f,-8.0f);
		glScalef(7.0f, 2.0f, 4.0f);
		glutSolidCube(1.0f);
	glPopMatrix();

// Far dret
	glPushMatrix();
		glTranslatef(13.5f,-17.0f,-8.0f);
		glScalef(7.0f, 2.0f, 4.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f); // Per a la textura
		glutSolidCube(1.0f);
	glPopMatrix();

// Textura frontal fars
	  if (textu)  
		{	GLfloat sPlane[4] = { 0.15f, 0.0f, 0.0f, 0.5f };
	  		GLfloat tPlane[4] = { 0.0f, 0.35f, 0.25f, 0.15f };
// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[4]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
				
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
			}

// Color frontal fars sense textura
	glColor4f(1.0f,1.0f,1.0f,0.5f);

// Frontal far dret
	glPushMatrix();
		glTranslatef(-13.5f,-16.5f,-8.0f);
		glBegin(GL_QUADS);
			glNormal3f(0.0f,-1.0f,0.0f);	
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(-3.5f , -2.0f , -2.0f );
			glTexCoord2f(1.0f,0.00f);
			glVertex3f(3.5f , -2.0f , -2.0f );
			glTexCoord2f(1.0f,1.0);
			glVertex3f(3.5f , -2.0f , 2.0f );
			glTexCoord2f(0.0f,1.0f);	
			glVertex3f(-3.5f , -2.0f , 2.0f );
		glEnd();	
	glPopMatrix();

// Frontal far esquerre
	glPushMatrix();
		glTranslatef(13.5f,-16.5f,-8.0f);
		glBegin(GL_QUADS);
			glNormal3f(0.0f,-1.0f,0.0f);	
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(-3.5f , -2.0f , -2.0f );
			glTexCoord2f(1.0f,0.00f);
			glVertex3f(3.5f , -2.0f , -2.0f );
		glTexCoord2f(1.0f,1.0);
			glVertex3f(3.5f , -2.0f , 2.0f );
			glTexCoord2f(0.0f,1.0f);	
			glVertex3f(-3.5f , -2.0f , 2.0f );
		glEnd();
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// VIDRE CABINA (SENSE TEXTURA)
// Activar transpar�ncia
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

// Color vidre cabina sense textura
	glColor4f(0.3f,0.8f,0.7f,0.5f);

// Vidre
	glPushMatrix() ;
	  glTranslatef(0.0f,22.0f,10.0f);
	  glRotatef(-90.0f,1.0f,0.0f,0.0f);
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
	  glTranslatef(0.0f,00.0f,-10.0f);

   	  glBegin(GL_QUADS);  // Cara de 4 costats
	 	glNormal3f(200.0f, 200.0f, 0.0f);
		glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
		glVertex3f(10.0f , 0.0f , 20.0f ); //P2
		glVertex3f(0.0f , -10.0f ,20.0f ); //P3
		glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle d'abaix
		glNormal3f(0.0f, 0.0f, -200.0f);
		glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
		glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
		glVertex3f(0.0f , -10.0f , 0.0f ); // P4
	  glEnd();

	  glBegin(GL_POLYGON);  // Triangle de dalt
		glNormal3f(0.0f, 0.0f, 200.0f);
		glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
	    glVertex3f(0.0f , -10.0f , 20.0f ); // P3
		glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
	  glEnd();
	glPopMatrix();

// Desactivar transpar�ncia
	glDisable(GL_BLEND);

// TUBS D'ESCAPAMENT (SENSE TEXTURA)
// Color tub d'escapament sense textura
	glColor4f(1.0f,0.0f,0.0f,0.5f);

// Tub d'escapament esquerre
	glPushMatrix();
		glTranslatef(18.0f,42.0f,10.0f);	
		gluCylinder(2.0f, 2.0f, 40.0f, 12, 1);
	glPopMatrix();
	
// Tub d'escapament dret
	glPushMatrix();
		glTranslatef(-18.0f,42.0f,10.0f);	
		gluCylinder(2.0f, 2.0f, 40.0f, 12, 1);
	glPopMatrix();

// CARROSSERIA
// Textura carrosseria
	if (textu)
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color porta-eix motriu sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu mig
	glPushMatrix();
	  glTranslatef(0.0f,82.0f,-5.0f);
	  glScalef(1.0f, 5.0f, 1.0f);
	  glutSolidCube(10.0f);
	glPopMatrix();

	if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix mig
	glPushMatrix();
// Despla�ament Eix Mig
	  glTranslatef(0.0f,72.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0f, 2.0f, 26.0f, 6, 1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Mig Davanter
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Despla�ament entre dos eixos
	  glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0, 2.0, 26.0f, 6, 1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Mig Darrera
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Fi conjunt d'eixos i rodes eix mig
    glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);
		
// REMOLC	
// Textura carrosseria
	if (textu)
		{	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
			glBindTexture(GL_TEXTURE_2D,VTextu[5]);
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}


// Color plataforma sense textura
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Plataforma
	glPushMatrix();
		glTranslatef(0.0f,127.0f,4.0f);
		glScalef(3.0f, 14.0f, 0.8f);
		glutSolidCube(10.0f);
	glPopMatrix();

// Textura Cisterna
	if (textu)
		{	glBindTexture(GL_TEXTURE_2D,VTextu[6]);
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);			
		}

// Color cisterna sense textura
	glColor4f(0.9f,0.9f,1.0f,0.5f);

// Cisterna
	glPushMatrix();
	  glTranslatef(0.0f,60.0f,20.0f);
	  glScalef(0.75f,1.0f,0.5f);
	  glRotatef(270.0f,1.0f,0.0f,0.0f);
// Part de davant
	  gluCylinder(20.0f, 25.0f, 5.0f, 25, 1);
	  glPushMatrix();
	  	glRotatef(180.0f,1.0f,0.0f,0.0f);
		gluDisk(0.0f, 20.0f, 25, 1);	
	  glPopMatrix();
// Part Central
	  glTranslatef(0.0f,0.0f,5.0f);
	  gluCylinder(25, 25, 125.0f, 25, 1);
// Part Darrera
	  glPushMatrix();
		glTranslatef(0.0f,0.0f,125.0f);
		gluCylinder(25.0f, 20.0f, 5.0f, 25, 1);
		glTranslatef(0.0f,0.0f,5.0f);
		gluDisk(0.0f, 20.0f, 25, 1);	
	  glPopMatrix();
	glPopMatrix();

// Desactivar textura
//	if (textu) glDisable(GL_TEXTURE_2D);

// Textura planxa
	if (textu)  
		{	
			GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
		  	GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
	
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
		}

// Color porta-eix motriu
	glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu darrera
	glPushMatrix();
	  glTranslatef(0.0f,172.0f,-5.0f);
	  glScalef(1.0f, 5.0f, 1.0f);
	  glutSolidCube(10.0f);
	glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix darrera
	glPushMatrix();

// Despla�ament Eix Darrera
	  glTranslatef(0.0f,162.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(0.0f,0.0f,-10.0f);
	    glRotatef(90.0f,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0,2.0,26.0f,6,1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Davanter
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Desactivar textura
	if (textu) glDisable(GL_TEXTURE_2D);

// Despla�ament entre dos eixos
	  glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
	  glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Darrera
	  glPushMatrix();
		glTranslatef(0.0f,0.0f,-10.0f);
	 	glRotatef(90.0f,0.0f,1.0f,0.0f);
		glTranslatef(0.0f,0.0f,-13.0f);
		gluCylinder(2.0f,2.0f,26.0f,6,1);
	  glPopMatrix();
	
// Roda Dreta Davantera Eix Darrera Darrera
	  glPushMatrix();
	    glTranslatef(-10.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Darrera
	  glPushMatrix();
	    glTranslatef(17.0f,0.0f,-10.0f);
	    neumatic(textu,VTextu);
	  glPopMatrix();
// Fi conjunt eixos i rodes eix darrera
	glPopMatrix();

// Fi dibuix Truck
glPopMatrix();

// Restaurar textura[0]
if (textu)
	{	glBindTexture(GL_TEXTURE_2D,VTextu[0]);
// Desactivar genaraci� autom�tica de coordenades textura
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
//		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
		glEnable(GL_TEXTURE_2D);			
	}
}


// Dibuix de roda
// neumatic: Par�metres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void neumatic(bool textur, GLint VTextur[NUM_MAX_TEXTURES])
{
// Textura neum�tic
	if (textur)  
		{	
// Activar textura neum�tic
			glBindTexture(GL_TEXTURE_2D,VTextur[1]) ;
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);
		}

// Color neum�tic sense textura
	glColor4f(0.2f,0.2f,0.2f,0.5f);	

	glPushMatrix();
// Tapa inferior neum�tic
	  glRotatef(90.0f,0.0f,1.0f,0.0f);
	  gluDisk(5.0f, 10.0f, RESOLUCIO_RODA, 1);

// Dibuix de la llanta
	  llanta(textur,VTextur);

// Textura neum�tic
	  if (textur)  
		{	
// Activar textura neum�tic
			glBindTexture(GL_TEXTURE_2D,VTextur[1]) ;	
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glEnable(GL_TEXTURE_2D);
		}

// Color neum�tic sense textura
	glColor4f(0.2f,0.2f,0.2f,0.5f);	

// Gruix del neum�tic
	  glTranslatef(0.0f,0.0f,-6.0f);
	  gluCylinder(10.0f,10.0f,6.0f,RESOLUCIO_RODA,1);
// Tapa superior neum�tic
	  glRotatef(180.0f,0.0f,1.0f,0.0f);
	  gluDisk(5.0f,10.0f,RESOLUCIO_RODA,1);
// Dibuix de la llanta
	  llanta(textur,VTextur);
	glPopMatrix();

// Desactivar textures
//	if (textur) glDisable(GL_TEXTURE_2D);

}

// Dibuix de la llanta de la roda
// llanta: Par�metres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void llanta(bool textur, GLint VTextur[NUM_MAX_TEXTURES])
{

// Textura de la llanta
	if (textur)  
		{	GLfloat sPlane[4] = { 0.0f, -0.1f, 0.5f, 0.5f };
		  	GLfloat tPlane[4] = { 0.1f, 0.0f, 0.5f, 0.5f };
// Activar la textura
			glBindTexture(GL_TEXTURE_2D,VTextur[2]) ;
		
			glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
			
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_TEXTURE_2D);
		}

// Dibuix de la llanta (disc de radi 5 de color clar)
	glColor4f(0.7f,0.7f,0.7f,0.5f);	
	gluDisk(0.0f,5.0f,RESOLUCIO_RODA,1);

}


// Mar amb ondulacions
void sea(void)
{
	int i, j, step, it1, it2;
//     GLfloat pi=3.14159;
	float angle, delay;
	CColor color;

// Aigua amb ondulacions simulades a partir de normals sinusoidals
	step = 5;
	delay = 0;
	it1 = 0;

	//  SeleccionaMaterial(MAT_DEFAULT);	
	float h = 2 * pi*step / 500;
	for (j = -250; j<250 - step; j = j + step)
	{

		delay = 1.0*h*it1;

		it2 = 0;
		//glColor3f(0.5f, 0.4f, 0.9f);
		color.r = 0.5f;	color.g = 0.4f;	color.b = 0.9f;
		SeleccionaColor(reflexio_material, sw_materials, color);
	
		for (i = -250; i<250 - step; i = i + step)
		{
			glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i, j, 0);

			  angle = 1.0*(it2 + 1)*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i + step, j, 0);

			  angle = 1.0*(it2 + 1)*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i + step, j + step, 0);
			glEnd();

			glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i, j, 0);

			  angle = 1.0*(it2 + 1)*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i + step, j + step, 0);

			  angle = 1.0*it2*h * 15;
			  glNormal3f(-cos(angle), 0, 1);
			  glVertex3f(i, j + step, 0);
			glEnd();
			it2++;
		}
		it1++;
	}

}


// OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------

// Objecte TIE
void tie(){
	Motor();
	Alas();
	Canon();
	Cuerpo();
	Cabina();
};

void Alas()
{
// Ala 1 Parte central

//Parte Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	glPopMatrix();

	//Tapas
	glPushMatrix();
//	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
//	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(27.75f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Parte Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslated(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(2.0f, 2.0f, 2.0f);
 	  gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	glPopMatrix();

//Tapas
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Decoracion Exterior
	int j;
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(26.5f, 0.0f, 0.0f);
	  glScalef(15.75f, 13.75f, 13.75f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  for (j = 0; j<3; j = j + 1)
	{	glRotatef(60, 0, 0, 1);
		glPushMatrix();
		  glScalef(12.7f * 0.583f, 0.1f, 0.3f);
		  glutSolidCube(1.0f);
		glPopMatrix();
	}
	glPopMatrix();

	int u;
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	for (u = 0; u < 6; u = u + 1)
	{	glRotated(60, 1.0, 0.0, 0.0);
		glPushMatrix();
		  glTranslatef(26.5f, 0.0f, 43.25f);
		  glRotatef(90, 0.0, 0.0, 1.0);
		  glScalef(10 * 5.155f, 5.0f, 3.0f);
		  glutSolidCube(1.0f);
		glPopMatrix();
	}
	glPopMatrix();

//Ala 2 Parte central 

//Parte Exterior
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslated(-27.75f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(10.0, 10.0, 5.0);
	  gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	glPopMatrix();

//Tapas
	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(-27.75f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  glTranslatef(-25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(10.0f, 10.0f, 5.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Parte Interior
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	glPopMatrix();

//Tapas
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
 	  glTranslatef(-29.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-25.25f, 0.0f, 0.0f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(2.0f, 2.0f, 2.0f);
	  gluDisk(0.0f, 5.0f, 6, 1);
	glPopMatrix();

//Decoracion Exterior
	int w;
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  glTranslatef(-26.5f, 0.0f, 0.0f);
	  glScalef(15.75f, 13.75f, 13.75f);
	  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  for (w = 0; w<3; w = w + 1)
	  {	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		  glScalef(12.7f * 0.583f, 0.1f, 0.3f);
		  glutSolidCube(1.0f);
		glPopMatrix();
	}
	glPopMatrix();


	int h;
	glPushMatrix();
	  glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  for (h = 0; h < 6; h = h + 1)
	  {	glRotated(60, 1.0, 0.0, 0.0);
		glPushMatrix();
		  glTranslatef(-26.5f, 0.0f, 43.25f);
		  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		  glScalef(10 * 5.155f, 5.0f, 3.0f);
		  glutSolidCube(1.0f);
		glPopMatrix();
	  }
	glPopMatrix();
};


void Motor()
{
//Helices Motor
	int j;
	glPushMatrix();
	  glColor4f(0.58f, 0.58f, 0.58f, 0.0f);
	  glTranslatef(0.0f, -18.0f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glRotatef(45, 0, 0, 1);
	  for (j = 0; j<6; j = j + 1)
	  {	glRotatef(30, 0, 0, 1);
		glPushMatrix();
		  glScalef(12 * 0.583f, 0.1f, 0.3f);
		  glutSolidCube(1.0f);
		glPopMatrix();
	  }
	glPopMatrix();

//Soporte Motor
	glPushMatrix();
	  glColor4f(0.28f, 0.28f, 0.28f, 0.0f);
	  glTranslatef(0.0f, -18.0f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glutSolidTorus(1.0, 5.0, 20, 20);
	glPopMatrix();
};

void Canon()
{

//Ca�ones

// Salida ca�on 1
	glPushMatrix();
	  glColor4f(0.28f, 0.28f, 0.28f, 0.0f);
	  glTranslated(5.0f, 8.0f, -10.0f);
	  glRotated(-90.0f, 1.0f, 0.0f, 0.0f);
	  gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	glPopMatrix();

// Salida ca�on 2
	glPushMatrix();
	  glColor4f(0.28f, 0.28f, 0.28f, 0.0f);
	  glTranslated(-5.0f, 8.0f, -10.0f);
	  glRotated(-90.0f, 1.0f, 0.0f, 0.0f);
	  gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	glPopMatrix();

// Ca�on 1
	glPushMatrix();
	  glColor4f(0.58f, 0.58f, 0.58f, 0.0f);
	  glTranslated(5.0f, 10.0f, -10.0f);
	  glRotated(-90.0f, 1.0f, 0.0f, 0.0f);
	  //gluCylinder(ca�on1, 0.35f, 0.35f, 5.0f, 80, 1);
	  gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	glPopMatrix();

//Ca�on 2
	glPushMatrix();
	  glColor4f(0.58f, 0.58f, 0.58f, 0.0f);
 	  glTranslated(-5.0f, 10.0f, -10.0f);
	  glRotated(-90.0f, 1.0f, 0.0f, 0.0f);
	  gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	glPopMatrix();
}

void Cuerpo()
{
//Sujeccion de las Alas

//Lado2
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glRotated(-90.0f, 0.0, 1.0, 0.0);
	  glRotated(90.0f, 0.0, 0.0, 1.0);
	  glScalef(2.0f, 2.0f, 2.5f);
	  gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	glPopMatrix();

//X2
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glTranslated(-25.0f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glutSolidTorus(1.0, 5.0, 20, 20);
	glPopMatrix();

//Lado 1
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glRotated(90.0f, 0.0, 1.0, 0.0);
	  glRotated(90.0f, 0.0, 0.0, 1.0);
	  glScaled(2.0f, 2.0f, 2.5f);
	  gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	glPopMatrix();

//X1
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glTranslated(25.25f, 0.0f, 0.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glutSolidTorus(1.0, 5.0, 20, 20);
	glPopMatrix();

//Cuerpo
	glPushMatrix();
	  glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  glScaled(1.75f, 1.75f, 1.5f);
	  glutSolidSphere(10.0f, 80, 80);
	  //gluEsfera(10.0f, 80, 80);
	glPopMatrix();
}

void Cabina()
{
	glEnable(GL_BLEND);

// Tapa Cabina
	glPushMatrix();
	  glColor4f(1.0f, 0.058f, 0.058f, 0.5f);
	  glTranslated(0.0f, 19.45f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScalef(1.75f, 1.75f, 1.75f);
	  gluDisk(0.0f, 1.5f, 8, 1);
	glPopMatrix();

// Cristal Cabina
	glPushMatrix();
	  glColor4f(1.0f, 0.058f, 0.058f, 0.5f);
	  glTranslated(0.0f, 19.45f, 0.0f);
	  glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  glScaled(1.75f, 1.75f, 1.75f);
	  gluCylinder(1.5f, 4.5f, 2.0f, 8, 1);
	glPopMatrix();

	glDisable(GL_BLEND);
}
// FI OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------