//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2019 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2019)
// EntornVGIView.cpp: implementación de la clase CEntornVGIView
// FUNCIONS:		- Control del bucle principal (OnPaint)
//					- Control teclat (OnKeyDown)
//					- Control mouse interactiu i botons mouse 
//							(OnLButtomDown, OnRButtomDown, OnMouseMove)
//					- Control opcions de menú (On*, OnUpdate*)
//					- Control de color de fons per teclat (FONS)
//					- Transformacions Geomètriques Interactives via mouse
//
//    Versió 2.0:	- Canvi de color de l'objecte per teclat (OBJ)
//					- Fixar Transformacions Geomètriques desactivant opcions Transforma (INSERT dins opcions Transforma)
//					- Esborrar Transformacions Geomètriques fixades (DELETE dins opcions Transforma)
//					- Fixar Traslació pantalla fora de l'opció Vista->Pan? (INSERT dins opció Vista->Pan?)
//					- Esborrar Traslació pantalla fixada (DELETE dins opció Vista->Pan?)
//
//	  Versió 2.2:	- Opció VISTA --> Satèl.lit: Punt de Vista en moviment segons moviment mouse
//
//	  Versió 2.5:	- Afegits objectes cubRGB i Tie (nau Star Wars fet per alumnes)
//					- Opció VISTA --> FullScreen?: Opció FullScreen per menú i per PopUp menu enmig pantalla. Té un bug al restaurar pantalla 
//													normal (inclos menú IDR_MENU_WINDOW)
//

#include "stdafx.h"
//OBJ PATH
//extern const CString PATH_HURAKAN = CString(_T("obj/hurakan.obj"));
extern const CString PATH_ARM = CString(_T("obj/hurakan_arm.obj"));
extern const CString PATH_BASE = CString(_T("obj/hurakan_base.obj"));
extern const CString PATH_ASIENTO = CString(_T("obj/hurakan_seients.obj"));
extern const CString PATH_FLOOR = CString(_T("obj/floor.obj"));
extern const CString PATH_SKYDOME = CString(_T("obj/skydome.obj"));
extern const CString PATH_GRASS = CString(_T("obj/Background_grass.obj"));
extern const CString PATH_WALLS = CString(_T("obj/Background_walls.obj"));
extern const CString PATH_OTHERS = CString(_T("obj/Background_others.obj"));
extern const CString PATH_TEMPLE = CString(_T("obj/Background_temple.obj"));
extern const CString PATH_PERSONA = CString(_T("obj/hombre.obj"));
//TEXTURES
extern const CString PATH_TEXTURE_ARM = CString(_T("textures/Arm.png"));
extern const CString PATH_TEXTURE_BASE = CString(_T("textures/Base.png"));
extern const CString PATH_TEXTURE_SEIENTS = CString(_T("textures/Seients.png"));
extern const CString PATH_TEXTURE_SKYDOME = CString(_T("textures/philo_sky1_2k.jpg"));
extern const CString PATH_TEXTURE_FLOOR = CString(_T("textures/floor.jpg"));
extern const CString PATH_TEXTURE_GRASS = CString(_T("textures/grass.jpg"));
extern const CString PATH_TEXTURE_WALLS = CString(_T("textures/wall.png"));
extern const CString PATH_TEXTURE_OTHERS = CString(_T("textures/others.png"));
extern const CString PATH_TEXTURE_TEMPLE = CString(_T("textures/Temple.png"));
extern const CString PATH_TEXTURE_MANDO_ON = CString(_T("textures/mando_on.jpg"));
extern const CString PATH_TEXTURE_MANDO_OFF = CString(_T("textures/mando_desc.png"));
//extern const CString PATH_TEXTURE_PERSONA = CString(_T("textures/hombre.obj"));



// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "EntornVGI.h"
#endif

#include "EntornVGIDoc.h"
#include "EntornVGIView.h"
#include "visualitzacio.h"	// Include funcions de projeció i il.luminació
#include "escena.h"			// Include funcions d'objectes OpenGL

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntornVGIView

IMPLEMENT_DYNCREATE(CEntornVGIView, CView)

BEGIN_MESSAGE_MAP(CEntornVGIView, CView)
	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEntornVGIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_ARXIU_OBRIR_FRACTAL, &CEntornVGIView::OnArxiuObrirFractal)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_OBJ, &CEntornVGIView::OnArxiuObrirFitxerObj)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_3DS, &CEntornVGIView::OnArxiuObrirFitxer3ds)
	ON_COMMAND(ID_SHADERS_SENSE, &CEntornVGIView::OnShadersSense)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_SENSE, &CEntornVGIView::OnUpdateShadersSense)
	ON_COMMAND(ID_SHADERS_GOURAUD, &CEntornVGIView::OnShadersGouraud)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_GOURAUD, &CEntornVGIView::OnUpdateShadersGouraud)

	//CHANGED
	ON_COMMAND(ID_HURAKAN_HURAKAN, &CEntornVGIView::OnHurakan)
	ON_UPDATE_COMMAND_UI(ID_HURAKAN_HURAKAN, &CEntornVGIView::OnUpdateHurakan)
	ON_COMMAND(ID_PARTESHURAKAN_BRAZO, &CEntornVGIView::OnHurakanBrazos)
	ON_UPDATE_COMMAND_UI(ID_PARTESHURAKAN_BRAZO, &CEntornVGIView::OnUpdateHurakanBrazos)
	ON_COMMAND(ID_PARTESHURAKAN_BASE, &CEntornVGIView::OnHurakanBase)
	ON_UPDATE_COMMAND_UI(ID_PARTESHURAKAN_BASE, &CEntornVGIView::OnUpdateHurakanBase)
	ON_COMMAND(ID_PARTESHURAKAN_ASIENTOS, &CEntornVGIView::OnHurakanAsientos)
	ON_UPDATE_COMMAND_UI(ID_PARTESHURAKAN_ASIENTOS, &CEntornVGIView::OnUpdateHurakanAsientos)
	ON_COMMAND(ID_CICLOS_PRUEBA1, &CEntornVGIView::OnCiclosPrueba1)
	ON_UPDATE_COMMAND_UI(ID_CICLOS_PRUEBA1, &CEntornVGIView::OnUpdateCiclosPrueba1)

	ON_UPDATE_COMMAND_UI(ID_PARTESHURAKAN_SKYDOME, &CEntornVGIView::OnUpdateHurakanSkydome)
	ON_COMMAND(ID_PARTESHURAKAN_SKYDOME, &CEntornVGIView::OnHurakanSkydome)
	ON_UPDATE_COMMAND_UI(ID_PARTESHURAKAN_FLOOR, &CEntornVGIView::OnUpdateHurakanFloor)
	ON_COMMAND(ID_PARTESHURAKAN_FLOOR, &CEntornVGIView::OnHurakanFloor)


	ON_COMMAND(ID_CICLOS_PRUEBAGRABADA, &CEntornVGIView::OnCiclosPruebagrabada)
	ON_UPDATE_COMMAND_UI(ID_CICLOS_PRUEBAGRABADA, &CEntornVGIView::OnUpdateCiclosPruebagrabada)
	//CAMARAS
	ON_COMMAND(ID_CAMARAS_PERSONA, &CEntornVGIView::OnCamarasSeients)
	ON_UPDATE_COMMAND_UI(ID_CAMARAS_PERSONA, &CEntornVGIView::OnUpdateCamarasSeients)
	ON_COMMAND(ID_CAMARA_SPLIT, &CEntornVGIView::OnCamarasSplit)
	ON_UPDATE_COMMAND_UI(ID_CAMARA_SPLIT, &CEntornVGIView::OnUpdateCamarasSplit)
	ON_COMMAND(ID_CAMARAS_DEFAULT, &CEntornVGIView::OnCamarasDefault)
	ON_UPDATE_COMMAND_UI(ID_CAMARAS_DEFAULT, &CEntornVGIView::OnUpdateCamarasDefault)
	ON_COMMAND(ID_CAMARAS_EXTERIOR_FRONTAL, &CEntornVGIView::OnCamarasExteriorFrontal)
	ON_UPDATE_COMMAND_UI(ID_CAMARAS_EXTERIOR_FRONTAL, &CEntornVGIView::OnUpdateCamarasExteriorFrontal)
	ON_COMMAND(ID_CAMARAS_TEMPLE, &CEntornVGIView::OnCamarasTemple)
	ON_UPDATE_COMMAND_UI(ID_CAMARAS_TEMPLE, &CEntornVGIView::OnUpdateCamarasTemple)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construcción o destrucción de CEntornVGIView

CEntornVGIView::CEntornVGIView()
{
// TODO: agregar aquí el código de construcción
	d1 = Demo_prova();
//	int i = 0;

//------ Entorn VGI: Inicialització de les variables globals de CEntornVGIView
	int i;

// Entorn VGI: Variables de control per Menú Veure->Pantalla Completa 
	fullscreen = false;

// Entorn VGI: Variables de control per Menú Vista: canvi PV interactiu, Zoom, dibuixar eixos i grid 
	mobil = true;	zzoom = true;	satelit = false;	pan = false;	navega = false;		eixos = true;
	sw_grid = false;
	grid.x = false;	grid.y = false; grid.z = false;		grid.w = false;
	hgrid.x = 0.0;	hgrid.y = 0.0;	hgrid.z = 0.0;		hgrid.w = 0.0;

// Entorn VGI: Variables opció Vista->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// Entorn VGI: Variables de control de l'opció Vista->Navega?
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;

// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
	projeccio = CAP; cam = DEFAULT_CAM;
	objecte = CAP;

// Entorn VGI: Variables de control del menú Transforma
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;

	transX = false;	transY = false;	transZ = false;

// Entorn VGI: Variables de control per les opcions de menú Ocultacions
	front_faces = true;	test_vis = false;	oculta = false;		back_line = false;

// Entorn VGI: Variables de control del menú Iluminació		
	ilumina = FILFERROS;			ifixe = false;					ilum2sides = false;
// Reflexions actives: Ambient [1], Difusa [2] i Especular [3]. No actives: Emission [0]. 
	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;
	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;
	textura = false;				t_textura = CAP;				textura_map = true;
	for (i = 0; i < NUM_MAX_TEXTURES; i++) texturesID[i] = -1;

// Entorn VGI: Variables de control del menú Llums
// Entorn VGI: Inicialització variables Llums
	llum_ambient = true;
	for (i = 1; i<NUM_MAX_LLUMS; i++) llumGL[i].encesa = false;
	for (i = 0; i<NUM_MAX_LLUMS; i++) {
		llumGL[i].encesa = false;
		llumGL[i].difusa[0] = 1.0f;	llumGL[i].difusa[1] = 1.0f;	llumGL[i].difusa[2] = 1.0f;	llumGL[i].difusa[3] = 1.0f;
		llumGL[i].especular[0] = 1.0f; llumGL[i].especular[1] = 1.0f; llumGL[i].especular[2] = 1.0f; llumGL[i].especular[3] = 1.0f;
	}

// LLum 0: Átenuació constant (c=1), sobre l'eix Z, no restringida.
	llumGL[0].encesa = true;
	llumGL[0].difusa[0] = 1.0f;			llumGL[0].difusa[1] = 1.0f;			llumGL[0].difusa[2] = 1.0f;		llumGL[0].difusa[3] = 1.0f;
	llumGL[0].especular[0] = 1.0f;		llumGL[0].especular[1] = 1.0f;		llumGL[0].especular[2] = 1.0f;	llumGL[0].especular[3] = 1.0f;

	llumGL[0].posicio.R = 75.0;			llumGL[0].posicio.alfa = 90.0;		llumGL[0].posicio.beta = 0.0;		// Posició llum (x,y,z)=(0,0,75)
	llumGL[0].atenuacio.a = 0.0;		llumGL[0].atenuacio.b = 0.0;		llumGL[0].atenuacio.c = 1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
	llumGL[0].restringida = false;
	llumGL[0].spotdirection[0] = 0.0;	llumGL[0].spotdirection[1] = 0.0;	llumGL[0].spotdirection[2] = 0.0;
	llumGL[0].cutoff = 0.0;				llumGL[0].exponent = 0.0;

// LLum 1: Atenuació constant (c=1), sobre l'eix X, no restringida.
	llumGL[1].encesa = false;
	llumGL[1].difusa[0] = 1.0f;			llumGL[1].difusa[1] = 1.0f;			llumGL[1].difusa[2] = 1.0f;		llumGL[1].difusa[3] = 1.0f;
	llumGL[1].especular[0] = 1.0f;		llumGL[1].especular[1] = 1.0f;		llumGL[1].especular[2] = 1.0f;	llumGL[1].especular[3] = 1;

	llumGL[1].posicio.R = 75.0;			llumGL[1].posicio.alfa = 0.0;		llumGL[1].posicio.beta = 0.0;// (x,y,z)=(75,0,0)
	llumGL[1].atenuacio.a = 0.0;		llumGL[1].atenuacio.b = 0.0;		llumGL[1].atenuacio.c = 1.0;
	llumGL[1].restringida = false;
	llumGL[1].spotdirection[0] = 0.0;	llumGL[1].spotdirection[1] = 0.0;	llumGL[1].spotdirection[2] = 0.0;
	llumGL[1].cutoff = 0.0;				llumGL[1].exponent = 0.0;

// LLum 2: Atenuació constant (c=1), sobre l'eix Y, no restringida.
	llumGL[2].encesa = false;
	llumGL[2].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[2] = 1.0f;		llumGL[2].difusa[3] = 1.0f;
	llumGL[2].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[2] = 1.0f;	llumGL[2].especular[3] = 1;

	llumGL[2].posicio.R = 75.0;			llumGL[2].posicio.alfa = 0.0;		llumGL[2].posicio.beta = 90.0;	// (x,y,z)=(0,75,0)
	llumGL[2].atenuacio.a = 0.0;		llumGL[2].atenuacio.b = 0.0;		llumGL[2].atenuacio.c = 1.0;
	llumGL[2].restringida = false;
	llumGL[2].spotdirection[0] = 0.0;	llumGL[2].spotdirection[1] = 0.0;	llumGL[2].spotdirection[2] = 0.0;
	llumGL[2].cutoff = 0.0;				llumGL[2].exponent = 0.0;

// LLum 3: Atenuació constant (c=1), sobre l'eix Y=X, no restringida.
	llumGL[3].encesa = false;
	llumGL[3].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[3].difusa[2] = 1.0f;		llumGL[3].difusa[3] = 1.0f;
	llumGL[3].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[3].especular[2] = 1.0f;	llumGL[3].especular[3] = 1;

	llumGL[3].posicio.R = 75.0;			llumGL[3].posicio.alfa = 45.0;		llumGL[3].posicio.beta = 45.0;// (x,y,z)=(75,75,75)
	llumGL[3].atenuacio.a = 0.0;		llumGL[3].atenuacio.b = 0.0;		llumGL[3].atenuacio.c = 1.0;
	llumGL[3].restringida = false;
	llumGL[3].spotdirection[0] = 0.0;	llumGL[3].spotdirection[1] = 0.0;	llumGL[3].spotdirection[2] = 0.0;
	llumGL[3].cutoff = 0.0;				llumGL[3].exponent = 0.0;

// LLum 4: Atenuació constant (c=1), sobre l'eix -Z, no restringida.
	llumGL[4].encesa = false;
	llumGL[4].difusa[0] = 1.0f;			llumGL[4].difusa[1] = 1.0f;			llumGL[4].difusa[2] = 1.0f;		llumGL[4].difusa[3] = 1.0f;
	llumGL[4].especular[0] = 1.0f;		llumGL[4].especular[1] = 1.0f;		llumGL[4].especular[2] = 1.0f;	llumGL[4].especular[3] = 1;

	llumGL[4].posicio.R = 75.0;			llumGL[4].posicio.alfa = -90.0;		llumGL[4].posicio.beta = 0.0;// (x,y,z)=(0,0,-75)
	llumGL[4].atenuacio.a = 0.0;		llumGL[4].atenuacio.b = 0.0;		llumGL[4].atenuacio.c = 1.0;
	llumGL[4].restringida = false;
	llumGL[4].spotdirection[0] = 0.0;	llumGL[4].spotdirection[1] = 0.0;	llumGL[4].spotdirection[2] = 0.0;
	llumGL[4].cutoff = 0.0;				llumGL[4].exponent = 0.0;

// LLum #5:
	llumGL[5].encesa = false;
	llumGL[5].difusa[0] = 1.0f;			llumGL[5].difusa[1] = 1.0f;			llumGL[5].difusa[2] = 1.0f;		llumGL[5].difusa[3] = 1.0f;
	llumGL[5].especular[0] = 1.0f;		llumGL[5].especular[1] = 1.0f;		llumGL[5].especular[2] = 1.0f;	llumGL[5].especular[3] = 1;

	llumGL[5].posicio.R = 0.0;			llumGL[5].posicio.alfa = 0.0;		llumGL[5].posicio.beta = 0.0; // Cap posició definida
	llumGL[5].atenuacio.a = 0.0;		llumGL[5].atenuacio.b = 0.0;		llumGL[5].atenuacio.c = 1.0;
	llumGL[5].restringida = false;
	llumGL[5].spotdirection[0] = 0.0;	llumGL[5].spotdirection[1] = 0.0;	llumGL[5].spotdirection[2] = 0.0;
	llumGL[5].cutoff = 0.0;				llumGL[5].exponent = 0.0;

// LLum #6: Llum Vaixell, configurada a la funció vaixell() en escena.cpp.
	llumGL[6].encesa = false;
	llumGL[6].difusa[0] = 1.0f;			llumGL[6].difusa[1] = 1.0f;			llumGL[6].difusa[2] = 1.0f;		llumGL[6].difusa[3] = 1.0f;
	llumGL[6].especular[0] = 1.0f;		llumGL[6].especular[1] = 1.0f;		llumGL[6].especular[2] = 1.0f;	llumGL[6].especular[3] = 1;

	llumGL[6].posicio.R = 0.0;			llumGL[6].posicio.alfa = 0.0;		llumGL[6].posicio.beta = 0.0; // Cap posició definida, definida en funció vaixell() en escena.cpp
	llumGL[6].atenuacio.a = 0.0;		llumGL[6].atenuacio.b = 0.0;		llumGL[6].atenuacio.c = 1.0;
	llumGL[6].restringida = false;
	llumGL[6].spotdirection[0] = 0.0;	llumGL[6].spotdirection[1] = 0.0;	llumGL[6].spotdirection[2] = 0.0;
	llumGL[6].cutoff = 0.0;				llumGL[6].exponent = 0.0;

// LLum #7: Llum Far, configurada a la funció faro() en escena.cpp.
	llumGL[7].encesa = false;
	llumGL[7].difusa[0] = 1.0f;		llumGL[7].difusa[1] = 1.0f;			llumGL[7].difusa[2] = 1.0f;		llumGL[7].difusa[3] = 1.0f;
	llumGL[7].especular[0] = 1.0f;		llumGL[7].especular[1] = 1.0f;		llumGL[7].especular[2] = 1.0f;	llumGL[7].especular[3] = 1;

	llumGL[7].posicio.R = 0.0;			llumGL[7].posicio.alfa = 0.0;		llumGL[7].posicio.beta = 0.0; // Cap posició definida, definida en funció faro() en escena.cpp
	llumGL[7].atenuacio.a = 0.0;		llumGL[7].atenuacio.b = 0.0;		llumGL[7].atenuacio.c = 1.0;
	llumGL[7].restringida = false;
	llumGL[7].spotdirection[0] = 0.0;	llumGL[7].spotdirection[1] = 0.0;	llumGL[7].spotdirection[2] = 0.0;
	llumGL[7].cutoff = 0.0;				llumGL[7].exponent = 0.0;

// Entorn VGI: Variables de control del menú Shaders
	sw_shader = false;				shader_menu = CAP;				shader_program = 0;

// Entorn VGI: Variables de control dels botons de mouse
	m_PosEAvall = (0, 0);		m_PosDAvall = (0, 0);
	m_ButoEAvall = false;		m_ButoDAvall = false;
	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;

// Entorn VGI: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
	w = 0;				h = 0;								// Mides finestra
	w_old = 0;			h_old = 0;							// Copia mides finestre per a FullScreen
	OPV.R = 30.0;		OPV.alfa = 0.0;		OPV.beta = 90.0;	// Origen PV en esfèriques
	Vis_Polar = POLARZ;

// Entorn VGI: Color de fons i de l'objecte
	fonsR = true;		fonsG = true;		fonsB = true;
	c_fons.r = 0.0;		c_fons.g = 0.0;		c_fons.b = 0.0;
	sw_color = false;
	col_obj.r = 1.0;	col_obj.g = 1.0;	col_obj.b = 1.0;		col_obj.a = 1.0;

// Entorn VGI: Objecte 3DS
	Ob3DS = NULL;

// Entorn VGI: Objecte OBJ
	ObOBJ = NULL;
	
// VGI: OBJECTE --> Corba B-Spline i Bezier
	npts_T = 0;
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{
		PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	pas_CS = PAS_BSPLINE;
	sw_Punts_Control = false;

// Entorn VGI: Variables del Timer
	t = 0;			anima = false;

// Entorn VGI: Variables de l'objecte FRACTAL
	t_fractal = CAP;	soroll = 'C';
	pas = 64;			pas_ini = 64;
	sw_il = true;		palcolFractal = false;

// Entorn VGI: Altres variables
	mida = 1.0;			nom = "";		buffer = ""; Player1 = new CXBOXController(1);

// Entorn VGI: Inicialització de les llibreries DevIL per a la càrrega de textures i fitxers .3DS
	ilInit();					// Inicialitzar llibreria IL
	iluInit();					// Inicialitzar llibreria ILU
	ilutRenderer(ILUT_OPENGL);	// Inicialitzar llibreria ILUT per a OpenGL

// Entorn VGI: Definir desplegable per a Full Screen
	ContextMenu = new CMenu();
	if (!ContextMenu->LoadMenu(IDR_MENU_WINDOW))	AfxMessageBox(_T("Fail to create context menu"));
}

CEntornVGIView::~CEntornVGIView()
{
}

BOOL CEntornVGIView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: modificar aquí la clase Window o los estilos cambiando
//  CREATESTRUCT cs

// An OpenGL window must be created with the following flags and must not
// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// Impresión de CEntornVGIView

void CEntornVGIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEntornVGIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Preparación predeterminada

	return DoPreparePrinting(pInfo);
}

void CEntornVGIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar inicialización adicional antes de imprimir
}

void CEntornVGIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar limpieza después de imprimir
}

void CEntornVGIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Diagnósticos de CEntornVGIView

#ifdef _DEBUG
void CEntornVGIView::AssertValid() const
{
	CView::AssertValid();
}

void CEntornVGIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEntornVGIDoc* CEntornVGIView::GetDocument() const // La versión de no depuración está alineada
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEntornVGIDoc)));
	return (CEntornVGIDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Controladores de mensaje de CEntornVGIView

int CEntornVGIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

// TODO:  Agregue aquí su código de creación especializado

	//CDC* pDC = GetDC();
	//m_glRenderer.CreateGLContext(pDC);
	m_pDC = new CClientDC(this);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

// Entorn VGI: Format del pixel que millor s'ajusti al descrit en pfd
	//int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	int nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if (!nPixelFormat)
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// Activació format pixel per al contexte dispositiu
	//BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);
	BOOL bResult = SetPixelFormat(m_pDC->GetSafeHdc(), nPixelFormat, &pfd);

	if (!bResult) 
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// Entorn VGI: Creació contexte generació OpenGL
	// m_hRC=wglCreateContext(m_hDC);
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (!m_hRC)
	{	::MessageBoxW(NULL, L"Error en GL Rendering Context", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// OPENGL 2.0 - OPENGL 3.0 **********************************************************
	// --- OpenGL 3.x ---
	HGLRC tempContext = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), tempContext);

//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}

//InitAPI();

//	if (glewIsExtensionSupported("GLEW_ARB_vertex_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader NOT supported, proceeding\n"));
//
//	if (glewIsExtensionSupported("GLEW_ARB_fragment_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader NOT supported, proceeding\n"));
//
	if (glewIsSupported("GL_VERSION_2_0")) //(GLEW_VERSION_2_0)
	{	} //AfxMessageBox(_T("INFO: OpenGL 2.0 supported!. Proceed\n"));
	else
	{
		AfxMessageBox(_T("INFO: OpenGL 2.0 not supported!. Exit\n"));
		//return EXIT_FAILURE;
	}

	
	int major, minor;
	GetGLVersion(&major, &minor);

	if (major < 3 || (major == 3 && minor < 2))
		AfxMessageBox(_T("OpenGL 3.2 is not supported!. Exit\n"));
//	else 
//		AfxMessageBox(_T("OpenGL 3.2 is supported!. Proceed"));

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,// | WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, //0x9126 , 0x00000001,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB != NULL)
	{
		m_hrc = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	}

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

// Entorn VGI: Inicialització de funcions de shaders per a OpenGL 2.0
	InitAPI();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);

	if (!m_hrc)
	{
		AfxMessageBox(_T("OpenGL 3.x RC was not created!"));
		return false;
	}

// Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

	return true;
}

void CEntornVGIView::InitAPI()
{
/*	Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gràfica gravades en fitxer extensions.txt
	CString nomf;
	nomf = "extensions.txt";
	char *nomfitxer = CString2Char(nomf);
	char* str = (char*)glGetString(GL_VENDOR);
	FILE* f = fopen(nomfitxer, "w");
	if(f)	{	fprintf(f,"VENDOR: %s\n",str);
				str = (char*)glGetString(GL_RENDERER);
				fprintf(f, "RENDERER: %s\n", str);
				str = (char*)glGetString(GL_VERSION);
				fprintf(f, "VERSION: %s\n", str);
				str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				fprintf(f, "SHADING_LANGUAGE_VERSION: %s\n", str);
				str = (char*)glGetString(GL_EXTENSIONS);
				fprintf(f, "EXTENSIONS: %s\n", str);
				fclose(f);
			}
	*/

// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

	// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}

void CEntornVGIView::GetGLVersion(int* major, int* minor)
{
	// for all versions
	char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

	*major = ver[0] - '0';
	if (*major >= 3)
	{
		// for GL 3.x
		glGetIntegerv(GL_MAJOR_VERSION, major);		// major = 3
		glGetIntegerv(GL_MINOR_VERSION, minor);		// minor = 2
	}
	else
	{
		*minor = ver[2] - '0';
	}

	// GLSL
	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);	// 1.50 NVIDIA via Cg compiler
}


void CEntornVGIView::OnDestroy()
{
	CView::OnDestroy();

// TODO: Agregue aquí su código de controlador de mensajes
	CDC* pDC = GetDC();
	ReleaseDC(pDC);
}


void CEntornVGIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

// TODO: Agregue aquí su código de controlador de mensajes

// A resize event occured; cx and cy are the window's new width and height.
// Find the OpenGL change size function given in the Lab 1 notes and call it here

// Entorn VGI: MODIFICACIÓ ->Establim les mides de la finestra actual
	w = cx;
	h = cy;

}


void CEntornVGIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

		// TODO: Agregue aquí su código de controlador de comandos
	char* nom_braç = CString2Char(PATH_ARM);
	char* nom_base = CString2Char(PATH_BASE);
	char* nom_asiento = CString2Char(PATH_ASIENTO);
	char* nom_floor = CString2Char(PATH_FLOOR);
	char* nom_grass = CString2Char(PATH_GRASS);
	char* nom_others = CString2Char(PATH_OTHERS);
	char* nom_temple = CString2Char(PATH_TEMPLE);
	char* nom_walls = CString2Char(PATH_WALLS);
	char* nom_skydome = CString2Char(PATH_SKYDOME);
	char* nom_persona = CString2Char(PATH_PERSONA);
	

	char* nomTextureMandoOn = CString2Char(PATH_TEXTURE_MANDO_ON);
	char* nomTextureMandoOff = CString2Char(PATH_TEXTURE_MANDO_OFF);
	char* nomTextureArm = CString2Char(PATH_TEXTURE_ARM);
	char* nomTextureBase = CString2Char(PATH_TEXTURE_BASE);
	char* nomTextureSeient = CString2Char(PATH_TEXTURE_SEIENTS);
	char* nomTextureSkydome = CString2Char(PATH_TEXTURE_SKYDOME);
	char* nomTextureFloor = CString2Char(PATH_TEXTURE_FLOOR);
	char* nomTextureTemple = CString2Char(PATH_TEXTURE_TEMPLE);
	char* nomTextureWalls = CString2Char(PATH_TEXTURE_WALLS);
	char* nomTextureGrass = CString2Char(PATH_TEXTURE_GRASS);
	char* nomTextureOthers = CString2Char(PATH_TEXTURE_OTHERS);

	//Grabacio
	d1.setGrabacio(); //Prepara la grabación
	
	objecte = HURAKAN;
	projeccio = PERSPECT;
	mobil = true;
	ilumina = PLANA;
	oculta = true;
	textura = true;
	cam = DEFAULT_CAM;
	demo_on = false;
	anima = true;
	if (d1.instant != 0 && demo_on) {
		KillTimer(WM_TIMER);
	}
	SetTimer(WM_TIMER, d1.interval, NULL);
	d1.set_t_base();
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	


	
	texturesID[OBJECTEBRAC] = loadIMA_SOIL(nomTextureArm);
	texturesID[OBJECTEBASE] = loadIMA_SOIL(nomTextureBase);
	texturesID[OBJECTESEIENT] = loadIMA_SOIL(nomTextureSeient);
	texturesID[OBJECTESKYDOME] = loadIMA_SOIL(nomTextureSkydome);
	texturesID[OBJECTEFLOOR] = loadIMA_SOIL(nomTextureFloor);
	texturesID[OBJECTEGRASS] = loadIMA_SOIL(nomTextureGrass);
	texturesID[OBJECTETEMPLE] = loadIMA_SOIL(nomTextureTemple);
	texturesID[OBJECTEOTHERS] = loadIMA_SOIL(nomTextureOthers);
	texturesID[OBJECTEPAD_OFF] = loadIMA_SOIL(nomTextureMandoOff);
	texturesID[OBJECTEPAD_ON] = loadIMA_SOIL(nomTextureMandoOn);
	texturesID[OBJECTEWALLS] = loadIMA_SOIL(nomTextureWalls);

		if (ObOBJ == NULL) ObOBJ = new COBJModel;
		
		ObOBJ->LoadModel(nom_braç, OBJECTEBRAC, true);
		ObOBJ->LoadModel(nom_base, OBJECTEBASE, true);
		ObOBJ->LoadModel(nom_asiento, OBJECTESEIENT, true);
		ObOBJ->LoadModel(nom_floor, OBJECTEFLOOR, true);
		ObOBJ->LoadModel(nom_skydome, OBJECTESKYDOME, true);
		ObOBJ->LoadModel(nom_grass, OBJECTEGRASS, true);
		
		ObOBJ->LoadModel(nom_walls, OBJECTEWALLS, true);
		
		ObOBJ->LoadModel(nom_temple, OBJECTETEMPLE, true);
		ObOBJ->LoadModel(nom_others, OBJECTEOTHERS, true);
	
		//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
	OnShadersGouraud();

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	

	CDC* pDC = GetDC();
	//m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// Dibujo de CEntornVGIView

void CEntornVGIView::OnDraw(CDC* /*pDC*/)
{

// TODO: agregar aquí el código de dibujo para datos nativos

	CEntornVGIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


void CEntornVGIView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
// TODO: Agregue aquí su código de controlador de mensajes
	GLfloat vpv[3] = { 0.0, 0.0, 1.0 };

// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

// Cridem a les funcions de l'escena i la projecció segons s'hagi 
// seleccionat una projecció o un altra
	switch (projeccio)
	{
	case PERSPECT:
// PROJECCIÓ PERSPECTIVA
		if (cam != SPLIT_CAM) {
			glEnable(GL_SCISSOR_TEST);
			glScissor(0, 0, w, h);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
					// Desactivació del retall de pantalla
			eixos = true;
			// Definició de Viewport, Projecció i Càmara
			Projeccio_Perspectiva(0, 0, w, h, OPV.R);
			if (navega) {
				Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
			}
			else {
				n[0] = 0;		n[1] = 0;		n[2] = 0;
				if (cam == CAM_ASIENTOS) d1.get_pos_asientos(pos_asiento_x, pos_asiento_y, pos_asiento_z);
				Vista_Nuestra(cam, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid, pos_persona_x, pos_persona_y,altura_persona,pos_asiento_x,pos_asiento_y,pos_asiento_z,d1.cap_brac,d1.cap_seient,d1.pan_beta,d1.pan_alfa);
			}

			// Dibuix de l'Objecte o l'Escena
			glPushMatrix();
				configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
				dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
			glPopMatrix();
			glClear(GL_DEPTH_BUFFER_BIT);
			glPushMatrix();
				glScissor(0, 0, TAMANY_QUADRE_MANDO * w, TAMANY_QUADRE_MANDO * w);
				glViewport(0, 0, TAMANY_QUADRE_MANDO * w, w * TAMANY_QUADRE_MANDO);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0,TAMANY_QUADRE_MANDO*w,0, TAMANY_QUADRE_MANDO*w, 0, 2);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(0, 0, 1.99, 0, 0, 0, 0, 1, 0);
				glEnable(GL_TEXTURE_2D);

				glPushMatrix();
					if (Player1->IsConnected()) {
						glActiveTexture(texturesID[OBJECTEPAD_ON]);
						glBindTexture(GL_TEXTURE_2D, texturesID[OBJECTEPAD_ON]);
					}
					else {
						glActiveTexture(texturesID[OBJECTEPAD_OFF]);
						glBindTexture(GL_TEXTURE_2D, texturesID[OBJECTEPAD_OFF]);
					}
					glBegin(GL_QUADS);
						//glColor3f(1, 0, 0);
						glTexCoord2i(0, 0);
						glVertex2i(0, 0);
						glTexCoord2i(0, 1);
						glVertex2i(0, TAMANY_QUADRE_MANDO*w);
						glTexCoord2i(1, 1);
						glVertex2i(TAMANY_QUADRE_MANDO*w, TAMANY_QUADRE_MANDO*w);
						glTexCoord2i(1, 0);
						glVertex2i(TAMANY_QUADRE_MANDO*w, 0);
					glEnd();
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			glDisable(GL_SCISSOR_TEST);
			// Intercanvia l'escena al front de la pantalla
					//SwapBuffers(m_pDC->GetSafeHdc());
			
		}
		else if (cam == SPLIT_CAM) {
			glEnable(GL_SCISSOR_TEST);
			glScissor(0, 0, w, h);
			glViewport(0, 0, w, h);

			//avall esq
			glScissor(0, 0, w / 2, h / 2);
			//glViewport(0, 0, w / 2, h / 2);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
			Projeccio_Perspectiva(0, 0, w/2, h/2, OPV.R);
			if (navega) {
				Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
			}
			else {
				OPV.alfa = 30;
				OPV.beta = 270;
				pos_persona_x = 0;
				pos_persona_y = 15;
				altura_persona = 0.5;
				n[0] = 0;		n[1] = 0;		n[2] = 0;
				Vista_Nuestra(EXTERIOR_FRONTAL, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid, pos_persona_x, pos_persona_y,altura_persona, pos_asiento_x, pos_asiento_y, pos_asiento_z,d1.cap_brac,d1.cap_seient,d1.pan_beta, d1.pan_alfa);
				altura_persona = ALTURA_PERSONA_INI;
			}
			
			// Dibuix de l'Objecte o l'Escena
			glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
			glPopMatrix();

			//avall dreta
			glScissor(w/2, 0, w/2 , h / 2);
			//glViewport(w/2, 0, w/2 , h / 2);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
			Projeccio_Perspectiva(w/2, 0, w/2 , h/2, OPV.R);
			if (navega) {
				Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
			}
			else {
				n[0] = 0;		n[1] = 0;		n[2] = 0;

				OPV.alfa = 325;
				OPV.beta = 90;
				altura_persona = 17;
				pos_persona_x = 0;
				pos_persona_y = -15;
				Vista_Nuestra(EXTERIOR_FRONTAL, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid, pos_persona_x, pos_persona_y,altura_persona, pos_asiento_x, pos_asiento_y, pos_asiento_z,  d1.cap_brac, d1.cap_seient, d1.pan_beta, d1.pan_alfa);
				altura_persona = ALTURA_PERSONA_INI;
			}

			// Dibuix de l'Objecte o l'Escena
			glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
			glPopMatrix();

			//amunt esq
			glScissor(0, h / 2, w / 2, h / 2);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
			Projeccio_Perspectiva(0, h/2, w / 2, h / 2, OPV.R);
			if (navega) {
				Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
			}
			else {
				n[0] = 0;		n[1] = 0;		n[2] = 0;
				OPV.beta = 300;
				OPV.alfa = 20;
				pos_persona_y = 20;
				pos_persona_x = -10;
				Vista_Nuestra(EXTERIOR_FRONTAL, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid, pos_persona_x, pos_persona_y,altura_persona, pos_asiento_x, pos_asiento_y, pos_asiento_z, d1.cap_brac, d1.cap_seient, d1.pan_beta, d1.pan_alfa);
			}

			// Dibuix de l'Objecte o l'Escena
			glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
			glPopMatrix();

			//amunt dreta
			glScissor(w/2, h / 2, w / 2, h / 2);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
			Projeccio_Perspectiva(w/2, h / 2, w / 2, h / 2, OPV.R);
			if (navega) {
				Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
			}
			else {
				n[0] = 0;		n[1] = 0;		n[2] = 0;
				OPV.beta = 120;
				OPV.alfa = 60;
				OPV.R = 30;
				Vista_Nuestra(DEFAULT_CAM, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides, eixos, grid, hgrid, pos_persona_x, pos_persona_y,altura_persona, pos_asiento_x, pos_asiento_y, pos_asiento_z, d1.cap_brac, d1.cap_seient, d1.pan_beta, d1.pan_alfa);
			}

			// Dibuix de l'Objecte o l'Escena
			glPushMatrix();
			configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
			dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
			glPopMatrix();

			glDisable(GL_SCISSOR_TEST);

		}
		
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	default:
// Entorn VGI: Creació de la llista que dibuixarà els eixos Coordenades Món. Funció on està codi per dibuixar eixos	
		
		glNewList(EIXOS, GL_COMPILE);
		  // Dibuix dels eixos sense il.luminació
		  glDisable(GL_LIGHTING);
		  // Dibuixar eixos sense textures
		  glDisable(GL_TEXTURE_2D);
		  // Desactivar boira
		  glDisable(GL_FOG);
		  deixos();						// Funció que dibuixa els Eixos Coordenades Món
		  if (ilumina != FILFERROS) glEnable(GL_LIGHTING);
		  if (textura) glEnable(GL_TEXTURE_2D);
		glEndList();

// Crida a la funció Fons Blanc
		FonsB();

// Intercanvia l'escena al front de la pantalla
		SwapBuffers(m_pDC->GetSafeHdc());
		break;
}

// Entorn VGI: Activació el contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

//  Actualitzar la barra d'estat de l'aplicació amb els valors R,A,B,PVx,PVy,PVz
	Barra_Estat();
	
	
}


// configura_Escena: Funcio que configura els parametres de Model i dibuixa les
//                   primitives OpenGL dins classe Model
void CEntornVGIView::configura_Escena() {

// Aplicar Transformacions Geometriques segons persiana Transformacio i Quaternions
	instancia(transf, TG, TGF);
}

// dibuixa_Escena: Funcio que crida al dibuix dels diferents elements de l'escana
void CEntornVGIView::dibuixa_Escena() {

//	Dibuix geometria de l'escena amb comandes GL.
	dibuixa_EscenaGL(objecte, col_obj, true, sw_material, textura, texturesID, textura_map,
		npts_T, PC_t, pas_CS, sw_Punts_Control,d1,demo_on);

	void dibuixa_EscenaGL(char objecte, CColor col_object, bool ref_mat, bool sw_mat[4],
		bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map,
		int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC,Demo_prova d1,bool demo_on);
}

// Barra_Estat: Actualitza la barra d'estat (Status Bar) de l'aplicació amb els
//      valors R,A,B,PVx,PVy,PVz en Visualització Interactiva.
void CEntornVGIView::Barra_Estat()
{
	CString sss;
	CEsfe3D OPVAux;
	float PVx, PVy, PVz;
	//GLfloat color;

// Status Bar fitxer fractal
	if (nom != "") GetStatusBar().SetPaneText(0, nom);

// Càlcul dels valors per l'opció Vista->Navega
	if (projeccio != CAP && projeccio != ORTO) {
		if (navega)
		{
			OPVAux.R = sqrt(opvN.x*opvN.x + opvN.y*opvN.y + opvN.z*opvN.z);
			OPVAux.alfa = (asin(opvN.z / OPVAux.R) * 180) / pi;
			OPVAux.beta = (atan(opvN.y / opvN.x)) * 180 / pi;
		}
		else {
			OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
		}
	}
	else {
		OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
	}

// Status Bar R Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";	
		else if (projeccio==ORTO) buffer=" ORTO   ";
			else if (navega) buffer = " NAV   ";
			else buffer.Format(_T("%.1f"), OPVAux.R);
	sss = _T("R=") + buffer;
// Refrescar posició R Status Bar
	GetStatusBar().SetPaneText(1, sss);

// Status Bar angle alfa Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (navega) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.alfa);
	sss = _T("a=") + buffer;
// Refrescar posició angleh Status Bar
	GetStatusBar().SetPaneText(2, sss);

// Status Bar angle beta Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (navega) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.beta);
	sss = _T("ß=") + buffer;
// Refrescar posició anglev Status Bar
	GetStatusBar().SetPaneText(3, sss);

// Transformació PV de Coord. esfèriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
	if (navega) { PVx = opvN.x; PVy = opvN.y; PVz = opvN.z; }
	else {
		if (Vis_Polar == POLARZ) {
			PVx = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.alfa*pi / 180);
		}
		else if (Vis_Polar == POLARY) {
			PVx = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
		else {
			PVx = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
	}

// Status Bar PVx
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.x);
			else float2CString(PVx);
	sss = _T("PVx=") + buffer;
// Refrescar posició PVx Status Bar
	GetStatusBar().SetPaneText(4, sss);

// Status Bar PVy
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.y);
			else float2CString(PVy);
	sss = _T("PVy=") + buffer;
// Refrescar posició PVy Status Bar
	GetStatusBar().SetPaneText(5, sss);

// Status Bar PVz
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.z);
			else float2CString(PVz);
	sss = _T("PVz=") + buffer;
// Refrescar posició PVz Status Bar
	GetStatusBar().SetPaneText(6, sss);

// Status Bar per indicar el modus de canvi de color (FONS o OBJECTE)
	sss = " ";
	if (sw_grid) sss = "GRID";
		else if (pan) sss = " PAN";
			else if (navega) sss = " NAV";
				else if (sw_color) sss = " OBJ";
					else sss = "FONS";

// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(7, sss);

// Status Bar per indicar tipus de Transformació (TRAS, ROT, ESC)
	sss = " ";
	if (transf) {
		if (rota) sss = "ROT";
		else if (trasl) sss = "TRA";
		else if (escal) sss = "ESC";
	}
	else if ((!sw_grid) && (!pan) && (!navega)) 
	  {	// Components d'intensitat de fons que varien per teclat
		if ((fonsR) && (fonsG) && (fonsB)) sss = " RGB";
		else if ((fonsR) && (fonsG)) sss = " RG ";
		else if ((fonsR) && (fonsB)) sss = " R   B";
		else if ((fonsG) && (fonsB)) sss = "   GB";
		else if (fonsR) sss = " R  ";
		else if (fonsG) sss = "   G ";
		else if (fonsB) sss = "      B";
	  }
// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(8, sss);

// Status Bar dels paràmetres de Transformació, Color i posicions de Robot i Cama
	sss = " ";
	if (transf)
	{	if (rota)
		{	buffer.Format(_T("%.1f"), TG.VRota.x);
			sss = _T("  ") + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VRota.y);
			sss = sss + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VRota.z);
			sss = sss + buffer;
		}
		else if (trasl)
		{	buffer.Format(_T("%.1f"), TG.VTras.x);
			sss = _T("  ") + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VTras.y);
			sss = sss + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VTras.z);
			sss = sss + buffer;
		}
		else if (escal)
		{	buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.y);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = sss + buffer;
		}
	}
	else if ((!sw_grid) && (!pan) && (!navega))
	 {	// Color fons
		if (!sw_color)
		{	buffer.Format(_T("%.3f"), c_fons.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.b);
			sss = sss + buffer;
		}
		else
		{	// Color objecte
			buffer.Format(_T("%.3f"), col_obj.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.b);
			sss = sss + buffer;
		}
	}

// Refrescar posició PVz Status Bar
	GetStatusBar().SetPaneText(9, sss);

// Status Bar per indicar el pas del Fractal
	if (objecte == O_FRACTAL)
	{	buffer.Format(_T("%.1d"), pas);
		sss = _T("Pas=") + buffer;
	}
	else { sss = "          "; }

// Refrescar posició Transformacions en Status Bar
	GetStatusBar().SetPaneText(10, sss);
}


void CEntornVGIView::float2CString(float varf)
{
	float vfloat;
	vfloat = varf;
	if (abs(varf)<1.0) buffer.Format(_T("%.5f"), varf);
	else if (abs(varf)<99.0) buffer.Format(_T("%.4f"), varf);
	else if (abs(varf)<999.0) buffer.Format(_T("%.3f"), varf);
	else if (abs(varf)<9999.0) buffer.Format(_T("%.2f"), varf);
	else if (abs(varf)<99999.0) buffer.Format(_T("%.1f"), varf);
	else buffer.Format(_T("%.0f"), varf);

}


/* ------------------------------------------------------------------------- */
/*                                  SHADERS                                  */
/* ------------------------------------------------------------------------- */

/*****************************************************************************
* initializeShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
GLuint CEntornVGIView::initializeShaders(CString filename)
{
	GLuint v, f, p;
	char *vs = NULL, *fs = NULL;
	CString file_Vert, file_Frag;
	int info_log_length = 0, compile_result = 0;
	FILE *fd;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

// Llegir Vertex Shader <filename>.vert
	file_Vert = filename + _T(".vert");
	char* fitxer_Vert = CString2Char(file_Vert);
	vs = textFileRead(fitxer_Vert);

// Llegir Fragment Shader <filename>.vert
	file_Frag = filename + _T(".frag");
	char* fitxer_Frag = CString2Char(file_Frag);
	fs = textFileRead(fitxer_Frag);

// Si s'han trobats els fitxers, procedir.
	if ((vs != NULL) && (fs != NULL))
	{
		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);

		free(vs);
		free(fs);
		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
// Llista error de compilació del Vertex Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Vertex Shader"));

			// Volcar missatges error a fitxer Compile_Error.VERT
			if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
			{
				AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
				return false;
			}
			for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
			fclose(fd);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// Compilació Fragment Shader
		glCompileShader(f);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);

// Llista error de compilació del Vertex Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Fragment Shader"));

			// Volcar missatges error a fitxer Compile_Error.FRAG
			if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
			{
				AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
				return false;
			}
			for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
			fclose(fd);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// Creació del Program que inclou Vertex i Fragment Shader
		p = glCreateProgram();
		glAttachShader(p, v);	// Relaciona Vertex Shader al Program
		glAttachShader(p, f);	// relaciona Fragment Shader al Program

// Linkedició del Program
		glLinkProgram(p);

// Activació del Programa
		glUseProgram(p);

// Retorna referència al Program creat
		return p;
	}
	else return 0;	// retornar 0 si no s'ha trobat el fitxer.
}


/*****************************************************************************
* loadFileShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
GLuint CEntornVGIView::loadFileShaders(CString file_Vert, CString file_Frag)
{

	GLuint v, f, p;
	char *vs = NULL, *fs = NULL;
	//	CString file_Vert, file_Frag;
	int info_log_length = 0, compile_result = 0;
	FILE *fd;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

//	file_Vert = filename + _T(".vert");
	char* fitxer_Vert = CString2Char(file_Vert);

	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

//	file_Frag = filename + _T(".frag");
	char* fitxer_Frag = CString2Char(file_Frag);

	fs = textFileRead(fitxer_Frag);
//fs = textFileRead(filename + ".frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE){
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error compilació Vertex Shader"));

		// Volcar missatges error a fitxer Compile_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			return false;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return 0;
	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE){
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error compilació Fragment Shader"));

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			return false;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(f); // Don't leak the shader.
		return 0;
	}

	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	glUseProgram(p);

	return p;
}


char *CEntornVGIView::textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {	AfxMessageBox(_T("Vertex or Fragment Program not found"));
				return NULL;
			}
	}
	return content;
}

/*****************************************************************************
* releaseAllShaders()
*      Unload shaders, return to OpenGL fixed function.
*****************************************************************************/
void CEntornVGIView::releaseAllShaders()
{
// Entorn VGI: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glUseProgram(0);

// Entorn VGI: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL TECLAT                              */
/* ------------------------------------------------------------------------- */

// OnKeyDown: Funció de tractament de teclat (funció que es crida quan es prem una tecla)
//   PARÀMETRES:
//    - nChar: Codi del caracter seleccionat
//    - nRepCnt: Nombre de vegades que s'ha apretat la tecla (acceleració)
//    - nFlags: Flags d'interrupció activats.
void CEntornVGIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	const float incr = 0.025f;
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

	if ((sw_grid) && ((grid.x) || (grid.y) || (grid.z))) Teclat_Grid(nChar, nRepCnt);
	else if (((nChar == 'G') || (nChar == 'g')) && ((grid.x) || (grid.y) || (grid.z))) sw_grid = !sw_grid;
		else if ((!pan) && (!transf) && (!navega))
				{	if (!sw_color) Teclat_ColorFons(nChar, nRepCnt);
						else Teclat_ColorObjecte(nChar, nRepCnt);
				}
				else {	if (transf)
							{	if (rota) Teclat_TransRota(nChar, nRepCnt);
									 else if (trasl) Teclat_TransTraslada(nChar, nRepCnt);
										else if (escal) Teclat_TransEscala(nChar, nRepCnt);
							}
						if (pan) Teclat_Pan(nChar, nRepCnt);
						else if (navega) Teclat_Navega(nChar, nRepCnt);
							else if (!sw_color) Teclat_ColorFons(nChar, nRepCnt);
								else Teclat_ColorObjecte(nChar, nRepCnt);
					}

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);


	//Brazo
	//Si pulsa Q, frena el brazo
	if (nChar == 81)
	{
		d1.setEstadoBrazo(FRENAR);
	}
	else if (nChar == 87) //Si pulsa W, acelera
	{
		d1.setEstadoBrazo(ACELERAR_POSITIVO);
	}
	else if (nChar == 83) //Si pulsa S acelera en el sentido contrario
	{
		d1.setEstadoBrazo(ACELERAR_NEGATIVO);
	}
	else if (nChar == 69) //Si pulsa E, espera
	{
		if (isWaiting)
		{
			d1.setEstadoBrazo(LIBRE);
			d1.setEstadoAsientos(LIBRE);
			isWaiting = false;
		}
		else
		{
			d1.setEstadoBrazo(PAUSAR);
			d1.setEstadoAsientos(PAUSAR);
			isWaiting = true;
		}
	}
	else if (nChar == 68) //Si pulsa D, clava el brazo (si puede)
	{
		if (isBrazoClavado)
		{
			d1.setEstadoBrazo(LIBRE);
			isBrazoClavado = false;
		}
		else
		{
			d1.setEstadoBrazo(CLAVAR_BRAZO);
			isBrazoClavado = true;
		}
	}

	//Grabación
	if (nChar == 71)	//Se inicia y se para con la G
	{
		if (isGrabando)
		{
			d1.stopGrabacio();
			isGrabando = false;
		}
		else
		{
			d1.initGrabacio();
			isGrabando = true;
		}
	}

	//Asientos
	if (nChar == 77)   //Si pulsas M giras positivamente los asientos
	{
		d1.setEstadoAsientos(GIRAR_POSITIVO);
	}
	else if (nChar == 66) //Si pulsas B giras negativamente los asientos 
	{
		d1.setEstadoAsientos(GIRAR_NEGATIVO);
	}
	else if (nChar == 78)	//Si pulsas N bloqueas los asientos 
	{
		if (isAsientoClavado)
		{
			isAsientoClavado = false;
			d1.setEstadoAsientos(LIBRE);
		}
		else
		{
			isAsientoClavado = true;
			d1.setEstadoAsientos(CLAVAR_ASIENTO);
		}
	}
	else if (nChar == 75) //Si pulsas K tambaleas
	{
		d1.setEstadoAsientos(TAMBALEAR);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEntornVGIView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado

	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	//Si levanta cualquiera de estas teclas cambia el brazo al modo libre
	if (nChar == 87 || nChar ==  83 || nChar == 81)
	{
		d1.setEstadoBrazo(LIBRE);
	}

	//Si levanta cualquiera de estas teclas cambia el asiento al modo libre
	if (nChar == 77 || nChar == 66)
	{
		d1.setEstadoAsientos(LIBRE);
	}
}


// Teclat_ColorObjecte: Teclat pels canvis de color de l'objecte per teclat.
void CEntornVGIView::Teclat_ColorObjecte(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
	if (objecte == O_FRACTAL)
	{
		if (nChar == 109) // Caràcter '-' 
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		 else if (nChar == 107) // Caràcter '+' 
		 {	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		 }
	}
	//	else 
	if (nChar == VK_DOWN) 
	{	if (fonsR) {	col_obj.r -= nRepCnt*incr;
						if (col_obj.r<0.0) col_obj.r = 0.0;
					}
		if (fonsG) {	col_obj.g -= nRepCnt*incr;
						if (col_obj.g<0.0) col_obj.g = 0.0;
					}
		if (fonsB) {	col_obj.b -= nRepCnt*incr;
						if (col_obj.b<0.0) col_obj.b = 0.0;
					}
	}
	else if (nChar == VK_UP) 
		{	if (fonsR) {	col_obj.r += nRepCnt*incr;
							if (col_obj.r>1.0) col_obj.r = 1.0;
						}
			if (fonsG) {	col_obj.g += nRepCnt*incr;
							if (col_obj.g>1.0) col_obj.g = 1.0;
						}
			if (fonsB) {	col_obj.b += nRepCnt*incr;
							if (col_obj.b>1.0) col_obj.b = 1.0;
						}
		}
		else if (nChar == VK_SPACE) 
			{	if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
														fonsB = false;
													}
				  else if ((fonsR) && (fonsG)) {	fonsG = false;
													fonsB = true;
												}
					else if ((fonsR) && (fonsB)) {	fonsR = false;
													fonsG = true;
												}
						else if ((fonsG) && (fonsB)) fonsR = true;
							else if (fonsR) {	fonsR = false;
												fonsG = true;
											}
								else if (fonsG) {	fonsG = false;
													fonsB = true;
												}
									else if (fonsB) {	fonsR = true;
														fonsG = true;
														fonsB = false;
													}
			}
				else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
					else if ((nChar == 'f') || (nChar == 'F')) sw_color = false;

}


// Teclat_ColorFons: Teclat pels canvis del color de fons.
void CEntornVGIView::Teclat_ColorFons(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
	if (objecte == O_FRACTAL)
	{	if (nChar == 109) // Caràcter '-' 
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		else if (nChar == 107) // Caràcter '+' 
		{	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		}
	}
	//	else 
	if (nChar == VK_DOWN) {
		if (fonsR) {	c_fons.r -= nRepCnt*incr;
						if (c_fons.r<0.0) c_fons.r = 0.0;
					}
		if (fonsG) {	c_fons.g -= nRepCnt*incr;
						if (c_fons.g<0.0) c_fons.g = 0.0;
					}
		if (fonsB) {	c_fons.b -= nRepCnt*incr;
						if (c_fons.b<0.0) c_fons.b = 0.0;
					}
		}
	else if (nChar == VK_UP) {
		if (fonsR) {	c_fons.r += nRepCnt*incr;
						if (c_fons.r>1.0) c_fons.r = 1.0;
					}
		if (fonsG) {	c_fons.g += nRepCnt*incr;
						if (c_fons.g>1.0) c_fons.g = 1.0;
					}
		if (fonsB) {	c_fons.b += nRepCnt*incr;
						if (c_fons.b>1.0) c_fons.b = 1.0;
					}
		}
	else if (nChar == VK_SPACE) {
		if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
												fonsB = false;
											}
			else if ((fonsR) && (fonsG)) {		fonsG = false;
												fonsB = true;
										}
				else if ((fonsR) && (fonsB)) {	fonsR = false;
												fonsG = true;
											}
					else if ((fonsG) && (fonsB)) fonsR = true;
						else if (fonsR) {	fonsR = false;
											fonsG = true;
										}
							else if (fonsG) {	fonsG = false;
												fonsB = true;
											}
								else if (fonsB) {	fonsR = true;
													fonsG = true;
													fonsB = false;
												}
		}
		else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
			else if ((nChar == 'f') || (nChar == 'F')) sw_color = false;
}

// Teclat_Navega: Teclat pels moviments de navegació.
void CEntornVGIView::Teclat_Navega(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

// Entorn VGI: Controls de moviment de navegació
	vdir[0] = n[0] - opvN.x;
	vdir[1] = n[1] - opvN.y;
	vdir[2] = n[2] - opvN.z;
	modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
	vdir[0] = vdir[0] / modul;
	vdir[1] = vdir[1] / modul;
	vdir[2] = vdir[2] / modul;
	switch (nChar)
	{	
	// Tecla cursor amunt
	case VK_UP:
		opvN.x += nRepCnt*fact_pan*vdir[0];
		opvN.y += nRepCnt*fact_pan*vdir[1];
		n[0] += nRepCnt*fact_pan*vdir[0];
		n[1] += nRepCnt*fact_pan*vdir[1];
		break;

	// Tecla cursor avall
	case VK_DOWN:
		opvN.x -= nRepCnt*fact_pan*vdir[0];
		opvN.y -= nRepCnt*fact_pan*vdir[1];
		n[0] -= nRepCnt*fact_pan*vdir[0];
		n[1] -= nRepCnt*fact_pan*vdir[1];
		break;

	// Tecla cursor esquerra
	case VK_LEFT:
		angleZ = +nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Tecla cursor dret
	case VK_RIGHT:
		angleZ = 360 - nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Tecla Inicio
	case VK_HOME:
		opvN.z += nRepCnt*fact_pan;
		n[2] += nRepCnt*fact_pan;
		break;

	// Tecla Fin
	case VK_END:
		opvN.z -= nRepCnt*fact_pan;
		n[2] -= nRepCnt*fact_pan;
		break;

	// Tecla PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<1) fact_pan = 1;
		break;

	// Tecla PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	default:
		break;
	}
}


// Teclat_Pan: Teclat pels moviments de Pan.
void CEntornVGIView::Teclat_Pan(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Tecla cursor amunt
	case VK_UP:
		tr_cpv.y -= nRepCnt*fact_pan;
		if (tr_cpv.y<-100000) tr_cpv.y = 100000;
		break;

	// Tecla cursor avall
	case VK_DOWN:
		tr_cpv.y += nRepCnt*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		break;

	// Tecla cursor esquerra
	case VK_LEFT:
		tr_cpv.x += nRepCnt*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		break;

	// Tecla cursor dret
	case VK_RIGHT:
		tr_cpv.x -= nRepCnt*fact_pan;
		if (tr_cpv.x<-100000) tr_cpv.x = 100000;
		break;

	// Tecla PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<0.125) fact_pan = 0.125;
		break;

	// Tecla PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	// Tecla Insert: Fixar el desplaçament de pantalla (pan)
	case VK_INSERT:
		// Acumular desplaçaments de pan (tr_cpv) en variables fixes (tr_cpvF).
		tr_cpvF.x += tr_cpv.x;		tr_cpv.x = 0.0;
		if (tr_cpvF.x>100000) tr_cpvF.y = 100000;
		tr_cpvF.y += tr_cpv.y;		tr_cpv.y = 0.0;
		if (tr_cpvF.y>100000) tr_cpvF.y = 100000;
		tr_cpvF.z += tr_cpv.z;		tr_cpv.z = 0.0;
		if (tr_cpvF.z>100000) tr_cpvF.z = 100000;
		break;

	// Tecla Delete: Inicialitzar el desplaçament de pantalla (pan)
	case VK_DELETE:
		// Inicialitzar els valors de pan tant de la variable tr_cpv com de la tr_cpvF.
		tr_cpv.x = 0.0;			tr_cpv.y = 0.0;			tr_cpv.z = 0.0;
		tr_cpvF.x = 0.0;		tr_cpvF.y = 0.0;		tr_cpvF.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransEscala: Teclat pels canvis del valor d'escala per X,Y,Z.
void CEntornVGIView::Teclat_TransEscala(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
// Modificar vector d'Escalatge per teclat (actiu amb Escalat únicament)
	// Tecla '+' (augmentar tot l'escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla '-' (disminuir tot l'escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla cursor amunt ('8')
	case 104:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		break;

	// Tecla cursor avall ('2')
	case 98:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		break;

	// Tecla cursor esquerra ('4')
	case 100:
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		break;

	// Tecla cursor dret ('6')
	case 102:
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		break;

	// Tecla HOME ('7')
	case 103:
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla END ('1')
	case 97:
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla INSERT
	case VK_INSERT:
		// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransRota: Teclat pels canvis del valor del vector de l'angle de rotació per X,Y,Z.
void CEntornVGIView::Teclat_TransRota(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Tecla cursor amunt ('8')
	case 104:
		TG.VRota.x += nRepCnt*fact_Rota;
		if (TG.VRota.x >= 360) TG.VRota.x -= 360;
		break;

	// Tecla cursor avall ('2')
	case 98:
		TG.VRota.x -= nRepCnt*fact_Rota;
		if (TG.VRota.x<1) TG.VRota.x += 360;
		break;

	// Tecla cursor esquerra ('4')
	case 100:
		TG.VRota.y -= nRepCnt*fact_Rota;
		if (TG.VRota.y<1) TG.VRota.y += 360;
		break;

	// Tecla cursor dret ('6')
	case 102:
		TG.VRota.y += nRepCnt*fact_Rota;
		if (TG.VRota.y >= 360) TG.VRota.y -= 360;
		break;

	// Tecla HOME ('7')
	case 103:
		TG.VRota.z += nRepCnt*fact_Rota;
		if (TG.VRota.z >= 360) TG.VRota.z -= 360;
		break;

	// Tecla END ('1')
	case 97:
		TG.VRota.z -= nRepCnt*fact_Rota;
		if (TG.VRota.z<1) TG.VRota.z += 360;
		break;

	// Tecla PgUp ('9')
	case 105:
		fact_Rota /= 2;
		if (fact_Rota<1) fact_Rota = 1;
		break;

	// Tecla PgDown ('3')
	case 99:
		fact_Rota *= 2;
		if (fact_Rota>90) fact_Rota = 90;
		break;

// Modificar vector d'Escalatge per teclat (actiu amb Rotació)
	// Tecla '+' (augmentar escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla '-' (disminuir escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla Insert: Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
	case VK_INSERT:
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;	TGF.VScal.y = 1.0;;	TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;	TGF.VRota.y = 0.0;	TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Espaiador
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}


// Teclat_TransTraslada: Teclat pels canvis del valor de traslació per X,Y,Z.
void CEntornVGIView::Teclat_TransTraslada(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

	switch (nChar)
	{
	// Tecla cursor amunt ('8')
	case 104:
		TG.VTras.x -= nRepCnt*fact_Tras;
		if (TG.VTras.x<-100000) TG.VTras.x = 100000;
		break;

	// Tecla cursor avall ('2')
	case 98:
		TG.VTras.x += nRepCnt*fact_Tras;
		if (TG.VTras.x>10000) TG.VTras.x = 100000;
		break;

	// Tecla cursor esquerra ('4')
	case 100:
		TG.VTras.y -= nRepCnt*fact_Tras;
		if (TG.VTras.y<-100000) TG.VTras.y = -100000;
		break;

	// Tecla cursor dret ('6')
	case 102:
		TG.VTras.y += nRepCnt*fact_Tras;
		if (TG.VTras.y>100000) TG.VTras.y = 100000;
		break;

	// Tecla HOME ('7')
	case 103:
		TG.VTras.z += nRepCnt*fact_Tras;
		if (TG.VTras.z>100000) TG.VTras.z = 100000;
		break;

	// Tecla END ('1')
	case 97:
		TG.VTras.z -= nRepCnt*fact_Tras;
		if (TG.VTras.z<-100000) TG.VTras.z = -100000;
		break;

	// Tecla PgUp ('9')
	case 105:
		fact_Tras /= 2;
		if (fact_Tras<1) fact_Tras = 1;
		break;

	// Tecla PgDown ('3')
	case 99:
		fact_Tras *= 2;
		if (fact_Tras>100000) fact_Tras = 100000;
		break;

// Modificar vector d'Escalatge per teclat (actiu amb Traslació)
	// Tecla '+' (augmentar escalat)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Tecla '-' (disminuir escalat)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Tecla INSERT
	case VK_INSERT:
		// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
	case VK_DELETE:
		// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Tecla Espaiador
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}


// Teclat_Grid: Teclat pels desplaçaments dels gridXY, gridXZ i gridYZ.
void CEntornVGIView::Teclat_Grid(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
		// Key Up cursor ('8')
	case 104:
		hgrid.x -= nRepCnt*PAS_GRID;
		break;

		// Key Down cursor ('2')
	case 98:
		hgrid.x += nRepCnt*PAS_GRID;
		break;

		// Key Left cursor ('4')
	case 100:
		hgrid.y -= nRepCnt*PAS_GRID;
		break;

		// Key Right cursor ('6')
	case 102:
		hgrid.y += nRepCnt*PAS_GRID;
		break;

		// Key HOME ('7')
	case 103:
		hgrid.z += nRepCnt*PAS_GRID;
		break;

		// Key END ('1')
	case 97:
		hgrid.z -= nRepCnt*PAS_GRID;
		break;

		// Key grid ('G')
	case 'G':
		sw_grid = !sw_grid;
		break;

		/*
		// Key grid ('g')
		case 'g':
		sw_grid = !sw_grid;
		break;
		*/

	default:
		break;
	}
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// OnLButtonDown: Funció que es crida quan s'apreta el botó esquerra del mouse.
void CEntornVGIView::OnLButtonDown(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado

// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
	m_ButoEAvall = true;
	m_PosEAvall = point;
	m_EsfeEAvall = OPV;
	CView::OnLButtonDown(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
void CEntornVGIView::OnLButtonUp(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
	m_ButoEAvall = false;
	
// OPCIÓ VISTA-->SATÈLIT: Càlcul increment desplaçament del Punt de Vista
	if ((satelit) && (projeccio != ORTO))
	{	//m_EsfeIncEAvall.R = m_EsfeEAvall.R - OPV.R;
		m_EsfeIncEAvall.alfa = 0.01f * (OPV.alfa - m_EsfeEAvall.alfa); //if (abs(m_EsfeIncEAvall.alfa)<0.01) { if ((m_EsfeIncEAvall.alfa)>0.0) m_EsfeIncEAvall.alfa = 0.01 else m_EsfeIncEAvall.alfa=0.01}
		m_EsfeIncEAvall.beta = 0.01f * (OPV.beta - m_EsfeEAvall.beta);
		if (abs(m_EsfeIncEAvall.beta) < 0.01) {
			if ((m_EsfeIncEAvall.beta) > 0.0) m_EsfeIncEAvall.beta = 0.01;
			else m_EsfeIncEAvall.beta = 0.01;
		}
		if ((m_EsfeIncEAvall.R == 0.0) && (m_EsfeIncEAvall.alfa == 0.0) && (m_EsfeIncEAvall.beta == 0.0)) KillTimer(WM_TIMER);
		else SetTimer(WM_TIMER, 10, NULL);
	}
	
	CView::OnLButtonUp(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan s'apreta el botó dret del mouse.
void CEntornVGIView::OnRButtonDown(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoDAvall
	m_ButoDAvall = true;
	m_PosDAvall = point;

	CView::OnRButtonDown(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan deixem d'apretar el botó dret del mouse.
void CEntornVGIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
	m_ButoDAvall = false;

//	ClientToScreen(&point);
//	OnContextMenu(this, point);

// Si fullscreen afegir menú per a restaurar Pantalla Normal en posició del cursor.
	if (fullscreen) {
		ClientToScreen(&point);
		if (ContextMenu->GetSubMenu(0)){
			CMenu *pSubMenu = ContextMenu->GetSubMenu(0);

			if (fullscreen)	pSubMenu->CheckMenuItem(ID_VISTA_FULLSCREEN, MF_CHECKED);
			else pSubMenu->CheckMenuItem(ID_VISTA_FULLSCREEN, MF_UNCHECKED);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				point.x, point.y, this);
		}
	}
}


// OnMouseMove: Funció que es crida quan es mou el mouse. La utilitzem per la 
//				  Visualització Interactiva amb les tecles del mouse apretades per 
//				  modificar els paràmetres de P.V. (R,angleh,anglev) segons els 
//				  moviments del mouse.
//      PARAMETRES: - nFlags: Flags que controlen si el botó es apretat o no.
//					- point: Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
void CEntornVGIView::OnMouseMove(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

// TODO: Add your message handler code here and/or call default
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{	
// Entorn VGI: Determinació dels angles (en graus) segons l'increment
//				horitzontal i vertical de la posició del mouse.
		CSize gir = m_PosEAvall - point;
		m_PosEAvall = point;
		if (projeccio == EXTERIOR_FRONTAL) {
			OPV.beta = OPV.beta + gir.cx / 2;
			OPV.alfa = OPV.alfa + gir.cy / 2;
		}
		else {
			OPV.beta = OPV.beta - gir.cx / 2;
			OPV.alfa = OPV.alfa + gir.cy / 2;
		}

// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
		if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360;
		if (OPV.alfa<0)		OPV.alfa = OPV.alfa + 360;
		if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360;
		if (OPV.beta<0)		OPV.beta = OPV.beta + 360;
		InvalidateRect(NULL, false);
	}
	else if (m_ButoEAvall && navega && (projeccio != CAP && projeccio != ORTO)) // Opció Navegació
	{
// Entorn VGI: Canviar orientació en opció de Navegació
		CSize girn = m_PosEAvall - point;
		angleZ = girn.cx / 2.0;
// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
		if (angleZ >= 360) angleZ = angleZ - 360;
		if (angleZ<0)	angleZ = angleZ + 360;

		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;

		m_PosEAvall = point;
		InvalidateRect(NULL, false);
	}

// Entorn VGI: Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse.
	else {
		bool transE = transX || transY;
		if (m_ButoEAvall && transE && transf)
		{
			// Calcular increment
			CSize girT = m_PosEAvall - point;
			if (transX)
			{	long int incrT = girT.cx;
				if (trasl)
				{	TG.VTras.x += incrT*fact_Tras;
					if (TG.VTras.x<-100000) TG.VTras.x = 100000;
					if (TG.VTras.x>100000) TG.VTras.x = 100000;
				}
				else if (rota)
				{	TG.VRota.x += incrT*fact_Rota;
					while (TG.VRota.x >= 360) TG.VRota.x -= 360;
					while (TG.VRota.x<0) TG.VRota.x += 360;
				}
				else if (escal)
				{	if (incrT<0) incrT = -1 / incrT;
					TG.VScal.x = TG.VScal.x*incrT;
					if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
					if (TG.VScal.x>8192) TG.VScal.x = 8192;
				}
			}
			if (transY)
			{	long int incrT = girT.cy;
				if (trasl)
				{	TG.VTras.y += incrT*fact_Tras;
					if (TG.VTras.y<-100000) TG.VTras.y = 100000;
					if (TG.VTras.y>100000) TG.VTras.y = 100000;
				}
				else if (rota)
				{	TG.VRota.y += incrT*fact_Rota;
					while (TG.VRota.y >= 360) TG.VRota.y -= 360;
					while (TG.VRota.y<0) TG.VRota.y += 360;
				}
				else if (escal)
				{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
										incrT = 1 / Log2(-incrT);
									}
					  else incrT = Log2(incrT);
					TG.VScal.y = TG.VScal.y*incrT;
					if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
					if (TG.VScal.y>8192) TG.VScal.y = 8192;
				}
			}
			m_PosEAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// Entorn VGI: Determinació del desplaçament del pan segons l'increment
//				vertical de la posició del mouse (tecla dreta apretada).
	if (m_ButoDAvall && pan && (projeccio != CAP && projeccio != ORTO))
	{
		CSize zoomincr = m_PosDAvall - point;
		long int incrx = zoomincr.cx;
		long int incry = zoomincr.cy;

// Desplaçament pan vertical
		tr_cpv.y -= incry*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		  else if (tr_cpv.y<-100000) tr_cpv.y = -100000;

// Desplaçament pan horitzontal
		tr_cpv.x += incrx*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		  else if (tr_cpv.x<-100000) tr_cpv.x = -100000;

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}
// Determinació del paràmetre R segons l'increment
//   vertical de la posició del mouse (tecla dreta apretada)
	//else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
	else if (m_ButoDAvall && zzoom && (projeccio != CAP))
	{	CSize zoomincr = m_PosDAvall - point;
		long int incr = zoomincr.cy / 1.0;

//		zoom=zoom+incr;
		OPV.R = OPV.R + incr;
		if (OPV.R<1) OPV.R = 1;
		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}
	else if (m_ButoDAvall && navega && (projeccio != CAP && projeccio != ORTO))
	{	// Avançar en opció de Navegació
		if (m_PosDAvall != point)
		{	CSize zoomincr = m_PosDAvall - point;

			float incr = zoomincr.cy / 2;
//			long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
			vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;
			opvN.x += incr*vdir[0];
			opvN.y += incr*vdir[1];
			n[0] += incr*vdir[0];
			n[1] += incr*vdir[1];
			m_PosDAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// Entorn VGI: Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse.
	else if (m_ButoDAvall && transZ && transf)
	{
		// Calcular increment
		CSize girT = m_PosDAvall - point;
		long int incrT = girT.cy;
		if (trasl)
		{	TG.VTras.z += incrT*fact_Tras;
			if (TG.VTras.z<-100000) TG.VTras.z = 100000;
			if (TG.VTras.z>100000) TG.VTras.z = 100000;
		}
		else if (rota)
		{	incrT = girT.cx;
			TG.VRota.z += incrT*fact_Rota;
			while (TG.VRota.z >= 360) TG.VRota.z -= 360;
			while (TG.VRota.z<0) TG.VRota.z += 360;
		}
		else if (escal)
		{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
								incrT = 1 / Log2(-incrT);
							}
			  else incrT = Log2(incrT);
			TG.VScal.z = TG.VScal.z*incrT;
			if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
			if (TG.VScal.z>8192) TG.VScal.z = 8192;
		}

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}

// Do not call CView::OnPaint() for painting messages
	CView::OnMouseMove(nFlags, point);
}

// OnMouseWheel: Funció que es crida quan es mou el rodet del mouse. La utilitzem per la 
//				  Visualització Interactiva per modificar el paràmetre R de P.V. (R,angleh,anglev) 
//				  segons el moviment del rodet del mouse.
//      PARAMETRES: - nFlags: Flags que controlen si el botó es apretat o no.
//					- zDelta: Unitats de desplaçament del rodet del mouse.
//					- pt: Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
BOOL CEntornVGIView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

	// Funció de zoom quan està activada la funció pan o les T. Geomètriques
	if ((zzoom) || (transX) || (transY) || (transZ))
	{
		OPV.R = OPV.R + zDelta / 4;
		if (OPV.R<1) OPV.R = 1;
		InvalidateRect(NULL, false);
	}
	else if (navega)
	{	vdir[0] = n[0] - opvN.x;
		vdir[1] = n[1] - opvN.y;
		vdir[2] = n[2] - opvN.z;
		modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
		vdir[0] = vdir[0] / modul;
		vdir[1] = vdir[1] / modul;
		vdir[2] = vdir[2] / modul;
		opvN.x += (zDelta / 4)*vdir[0];
		opvN.y += (zDelta / 4)*vdir[1];
		n[0] += (zDelta / 4)*vdir[0];
		n[1] += (zDelta / 4)*vdir[1];
		InvalidateRect(NULL, false);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/* ------------------------------------------------------------------------- */
/*					     TIMER (ANIMACIÓ)									 */
/* ------------------------------------------------------------------------- */
void CEntornVGIView::OnTimer(UINT_PTR nIDEvent)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	if (anima)	{
		// Codi de tractament de l'animació quan transcorren els ms. del crono.
		// Codi de tractament de l'animació quan transcorren els ms. del crono.

		d1.last_instant = d1.instant;
		d1.instant = (std::clock() - (double)d1.get_t_base()) / (double)CLOCKS_PER_SEC;
		if (d1.demo_on) {
			double aux = (std::clock() - (double)d1.get_start())/(double)CLOCKS_PER_SEC;
			if (aux > d1.duracion_total) {
				d1.demo_on = false;
				d1.reset_demo();
			}
		}
		d1.move_step();
		// Crida a OnPaint() per redibuixar l'escena
		InvalidateRect(NULL, false);
		//Control mando
		if (Player1->IsConnected())
		{
			XINPUT_STATE state = Player1->GetState();
			XINPUT_KEYSTROKE key = Player1->GetKey();
			bool tecla_brac = false;
			bool tecla_seient = false;
			//BOTONES BRAZO
			//Get moves brazo
			float brazo_pos = state.Gamepad.bRightTrigger;
			float brazo_neg = state.Gamepad.bLeftTrigger;
			bool refresh_pos = false;
			double time = d1.instant- d1.last_instant;
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
				if (cam != DEFAULT_CAM) {
					altura_persona = altura_persona - 2 * time;
				}
				else {
					OPV.R = OPV.R - 8*time;
				}
			}
			if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				if (cam != DEFAULT_CAM) {
					altura_persona = altura_persona + 2 * time;
				}
				else {
					OPV.R = OPV.R + 8*time;
				}
			}

			if (cam == EXTERIOR_FRONTAL) {
				double ang = 270-OPV.beta;
				if (ang < 0) ang = ang + 360;
				if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
					
					if (ang >= 0 && ang <= 90) {
						pos_persona_x = pos_persona_x - (1-ang/90) * (2*time);
						pos_persona_y = pos_persona_y + (ang/90)* (2*time);
					}
					else if (ang > 90 && ang <= 180) {
						pos_persona_y = pos_persona_y + (1-(ang-90)/90) * (3.5* time);
						pos_persona_x = pos_persona_x + ((ang - 90) / 90) * (3.5* time);
					}
					else if (ang > 180 && ang <= 270) {
						pos_persona_x = pos_persona_x + (1- (ang - 180) / 90) * (3.5* time);
						pos_persona_y = pos_persona_y - ((ang -180) / 90) * (3.5* time);
					}
					else if(ang > 270 && ang <= 360){
						pos_persona_y = pos_persona_y - (1 - (ang - 270) / 90) * (3.5* time);
						pos_persona_x = pos_persona_x - ((ang - 270) / 90) * (3.5* time);
					}
					refresh_pos = true;

				}
				if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
					
					if (ang >= 0 && ang <= 90) {
						pos_persona_x = pos_persona_x + (1-(ang / 90)) * (3.5* time);
						pos_persona_y = pos_persona_y - (ang/90) * (3.5* time);
					}
					else if (ang > 90 && ang <= 180) {
						pos_persona_y = pos_persona_y - (1 - (ang - 90) / 90)* (3.5* time);
						pos_persona_x = pos_persona_x - ((ang - 90) / 90) * (3.5* time);
					}
					else if (ang > 180 && ang <= 270) {
						pos_persona_x = pos_persona_x - (1 - (ang - 180) / 90)* (3.5* time);
						pos_persona_y = pos_persona_y + ((ang - 180) / 90) * (3.5* time);
					}
					else if(ang>270 && ang<=360){
						pos_persona_y = pos_persona_y + (1-(ang - 270) / 90) * (3.5* time);
						pos_persona_x = pos_persona_x + ((ang - 270) / 90) * (3.5* time);
					}
					refresh_pos = true;

				}

				if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {

					if (ang >= 0 && ang <= 90) {
						pos_persona_y = pos_persona_y - (1-(ang / 90)) * (3.5* time);
						pos_persona_x = pos_persona_x - (ang / 90) * (3.5* time);
					}
					else if (ang > 90 && ang <= 180) {
						pos_persona_x = pos_persona_x - (1 - ((ang-90) / 90)) * (3.5* time);
						pos_persona_y = pos_persona_y + ((ang - 90) / 90) * (3.5* time);
					}
					else if (ang > 180 && ang <= 270) {
						pos_persona_y = pos_persona_y + (1-((ang - 180) / 90)) * (3.5* time);
						pos_persona_x = pos_persona_x + ((ang - 180) / 90) * (3.5* time);
					}
					else if (ang > 270 && ang <= 360) {
						pos_persona_x = pos_persona_x + (1-((ang - 270) / 90)) * (3.5* time);
						pos_persona_y = pos_persona_y - ((ang - 270) / 90) * (3.5* time);
					}
					refresh_pos = true;
				}
				if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {

					if (ang >= 0 && ang <= 90) {
						pos_persona_y = pos_persona_y + (1 - (ang / 90)) * (3.5* time);
						pos_persona_x = pos_persona_x + (ang/90) * (3.5* time);
					}
					else if (ang > 90 && ang <= 180) {
						pos_persona_x = pos_persona_x + (1-(ang-90)/90) * (3.5* time);
						pos_persona_y = pos_persona_y - ((ang - 90) / 90) * (3.5* time);
					}
					else if (ang > 180 && ang <= 270) {
						pos_persona_y = pos_persona_y - (1 - (ang - 180) / 90) * (3.5* time);
						pos_persona_x = pos_persona_x - ((ang - 180) / 90) * (3.5* time);
					}
					else if(ang>270 && ang<=360) {
						pos_persona_x = pos_persona_x - (1 - (ang - 270) / 90) * (3.5* time);
						pos_persona_y = pos_persona_y + ((ang - 270) / 90) * (3.5* time);
					}
					refresh_pos = true;
				}
				if (refresh_pos) {
					InvalidateRect(NULL, false);
				}
			}

			if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && cam != SPLIT_CAM) {
				if (cam != DEFAULT_CAM && cam != CAM_ASIENTOS) {
					OPV.beta = OPV.beta - 40*time;
				}
				else if (cam == CAM_ASIENTOS) {
					d1.pan_beta = d1.pan_beta - 30 * time;
				}
				else {
					OPV.beta = OPV.beta + 40*time;
				}
				if (cam == TEMPLE_CAM) {
					if (OPV.beta >= 140)	OPV.beta = 140;
					if (OPV.beta <= 40) OPV.beta = 40;
				}
				else {
					if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360;
					if (OPV.beta < 0)		OPV.beta = OPV.beta + 360;
				}
				InvalidateRect(NULL, false);
				
			}
			if (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && cam != SPLIT_CAM) {
				if (cam != DEFAULT_CAM && cam != CAM_ASIENTOS) {
					OPV.beta = OPV.beta + 40*time;
				}
				else if (cam == CAM_ASIENTOS) {
					d1.pan_beta = d1.pan_beta + 30 * time;
				}
				else {
					OPV.beta = OPV.beta - 40*time;
				}

				if (cam == TEMPLE_CAM) {
					if (OPV.beta >= 140)	OPV.beta =140;
					if (OPV.beta <= 40) OPV.beta = 40;
				}
				else {
					if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360;
					if (OPV.beta < 0)		OPV.beta = OPV.beta + 360;
				}
				InvalidateRect(NULL, false);

			}
			
			if (state.Gamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && cam != SPLIT_CAM) {
			
				if (cam == CAM_ASIENTOS) {
					d1.pan_alfa = d1.pan_alfa + 30* time;
				}
				else {
					OPV.alfa = OPV.alfa + 30 * time;
				}
				if (cam == TEMPLE_CAM) {
					if (OPV.alfa >= 45)	OPV.alfa = 45;
					if (OPV.alfa <= -45) OPV.alfa = -45;
				}
				else if (cam==DEFAULT_CAM){
					if (OPV.alfa >= 180)	OPV.alfa = 180;
					if (OPV.alfa < 0)		OPV.alfa = 0;
				}
				else {
					if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360;
					if (OPV.alfa < 0)		OPV.alfa= OPV.alfa + 360;
				}
				InvalidateRect(NULL, false);

			}
			if (state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && cam != SPLIT_CAM ) {
				if (cam == CAM_ASIENTOS) {
					d1.pan_alfa = d1.pan_alfa - 30 * time;
				}
				else {
					OPV.alfa = OPV.alfa - 30 * time;
				}
				if (cam == TEMPLE_CAM) {
					if (OPV.alfa >= 45)	OPV.alfa = 45;
					if (OPV.alfa <= -45) OPV.alfa = -45;
				}
				else if (cam == DEFAULT_CAM) {
					if (OPV.alfa >= 180)	OPV.alfa = 180;
					if (OPV.alfa < 0)		OPV.alfa = 0;
				}
				else {
					if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360;
					if (OPV.alfa < 0)		OPV.alfa = OPV.alfa + 360;
				}
				InvalidateRect(NULL, false);

			}
			//Camara next
			if (key.VirtualKey == VK_PAD_DPAD_RIGHT) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (cam == DEFAULT_CAM) {
						cam = SPLIT_CAM;
					}
					else if (cam == SPLIT_CAM) {
						cam = CAM_ASIENTOS;
						OPV.alfa = 0;
						OPV.beta = 0;
						d1.pan_beta = 0;
					}
					else if (cam == CAM_ASIENTOS) {
						cam = TEMPLE_CAM;
						OPV.alfa = 0;
						OPV.beta = 90;
					}
					else if (cam == TEMPLE_CAM) {
						cam = EXTERIOR_FRONTAL;
						pos_persona_x = POS_PERSONA_INI_X;
						pos_persona_y = POS_PERSONA_INI_Y;
						OPV.alfa = 0;
						OPV.R = CAMP_VISIO_PERSONA;
						OPV.beta = 270;
					}
					else if (cam == EXTERIOR_FRONTAL) {
						cam = DEFAULT_CAM;
						OPV.alfa = 10;
						OPV.beta = 90;
					}
				}

			}
			//Camara ant
			if (key.VirtualKey == VK_PAD_DPAD_LEFT) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (cam == DEFAULT_CAM) {
						cam = EXTERIOR_FRONTAL;
						OPV.alfa = 0;
						OPV.beta = 270;
						OPV.R = CAMP_VISIO_PERSONA;
						pos_persona_x = POS_PERSONA_INI_X;
						pos_persona_y = POS_PERSONA_INI_Y;
					}
					else if (cam == EXTERIOR_FRONTAL) {
						cam = TEMPLE_CAM;
						OPV.alfa = 0;
						OPV.beta = 90;
					}
					else if (cam == TEMPLE_CAM) {
						cam = CAM_ASIENTOS;
						d1.pan_beta = 0;
						OPV.alfa = 0;
						OPV.beta = 0;
					}
					else if (cam == CAM_ASIENTOS) {
						cam = SPLIT_CAM;
					}
					else if (cam == SPLIT_CAM) {
						cam = DEFAULT_CAM;
						OPV.alfa = 10;
						OPV.beta = 90;
					}
				}

			}
			//Right Trigger--> Acelerar Brazo +
			if ((brazo_pos > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)&& !isBrazoClavado && !isWaiting) {
				d1.setEstadoBrazo(ACELERAR_POSITIVO);
				tecla_brac = true;
			}
			//Left Trigger--> Acelerar Brazo -
			if (brazo_neg > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && !tecla_brac && !isBrazoClavado && !isWaiting) {
				d1.setEstadoBrazo(ACELERAR_NEGATIVO);
				tecla_brac = true;
			}
			if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) && !tecla_brac && !isBrazoClavado && !isWaiting) {
				d1.setEstadoBrazo(FRENAR);
				tecla_brac = true;
			}
			//Clavar Brazo
			if ((key.VirtualKey == VK_PAD_A) && !tecla_brac && !isWaiting) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (!isBrazoClavado) {
						isBrazoClavado = true;
						d1.setEstadoBrazo(CLAVAR_BRAZO);
					}
					else {
						isBrazoClavado = false;
					}
				}

				tecla_brac = true;
			}

			
			//Asiento
			//Girar +
			if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !tecla_seient && !isAsientoClavado && !isWaiting) {
				d1.setEstadoAsientos(GIRAR_POSITIVO);
				tecla_seient = true;
			}
			//Girar Negativo
			if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && !tecla_seient && !isAsientoClavado && !isWaiting) {
				d1.setEstadoAsientos(GIRAR_NEGATIVO);
				tecla_seient = true;
			}
			//Frenar Asiento
			if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !tecla_seient && !isAsientoClavado && !isWaiting) {
				d1.setEstadoAsientos(FRENAR);
				tecla_seient = true;
			}
			//Tambaleo
			if ((key.VirtualKey == VK_PAD_DPAD_UP) && !tecla_seient && !isAsientoClavado && !isWaiting) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (!istambaleo) {
						istambaleo = true;
						d1.setEstadoAsientos(TAMBALEAR);
					}
					else {
						istambaleo = false;
					}
				}

				tecla_seient = true;
			}
			//Clavar Asiento
			if ((key.VirtualKey == VK_PAD_Y) && !tecla_seient && !isWaiting) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (!isAsientoClavado) {
						isAsientoClavado = true;
						d1.setEstadoAsientos(CLAVAR_ASIENTO);
					}
					else {
						isAsientoClavado = false;
					}
				}

				tecla_seient = true;
			}


			//PAUSA I RECORD
			if ((key.VirtualKey == VK_PAD_START) && !tecla_brac && !tecla_seient) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (!isWaiting) {
						isWaiting = true;
						d1.setEstadoAsientos(PAUSAR);
						d1.setEstadoBrazo(PAUSAR);
					}
					else {
						isWaiting = false;
					}
				}

			}
			else if ((key.VirtualKey == VK_PAD_BACK) && !tecla_brac && !tecla_seient && !isWaiting) {
				if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN) {
					if (!isGrabando) {
						isGrabando = true;
						d1.initGrabacio();
					}
					else {
						isGrabando = false;
						d1.stopGrabacio();
					}
				}

			}
			else if (!tecla_seient && !tecla_brac && !isWaiting && !istambaleo) {
				if (!isAsientoClavado)d1.setEstadoAsientos(LIBRE);
				if (!isBrazoClavado)d1.setEstadoBrazo(LIBRE);
			}
			
		}
		
		}
	else if (satelit)	{	// OPCIÓ SATÈLIT: Increment OPV segons moviments mouse.
		//OPV.R = OPV.R + m_EsfeIncEAvall.R;
		OPV.alfa = OPV.alfa + m_EsfeIncEAvall.alfa;
		while (OPV.alfa > 360) OPV.alfa = OPV.alfa - 360;	while (OPV.alfa < 0) OPV.alfa = OPV.alfa + 360;
		OPV.beta = OPV.beta + m_EsfeIncEAvall.beta;
		while (OPV.beta > 360) OPV.beta = OPV.beta - 360;	while (OPV.beta < 0) OPV.beta = OPV.beta + 360;

		// Crida a OnPaint() per redibuixar l'escena
		InvalidateRect(NULL, false);
		}

	CView::OnTimer(nIDEvent);
}


/* ------------------------------------------------------------------------- */
/*   RECURSOS DE MENU (persianes) DE L'APLICACIO:                            */
/*					1. ARXIUS												 */
/*					2. VEURE: ToolBar, StatusBar, Pantalla Completa			 */
/*					3. VISTA: Punt de Vista, Zoom, Pan Navega i Eixos	     */
/*					4. PROJECCIÓ                                             */
/*					5. OBJECTE					                             */
/*					6. TRANSFORMA											 */
/*					7. OCULTACIONS											 */
/*					8. IL.LUMINACIÓ											 */
/*					9. LLUMS												 */
/*				   10. SHADERS												 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*					1. ARXIUS 												 */
/* ------------------------------------------------------------------------- */

// Obrir fitxer Fractal
void CEntornVGIView::OnArxiuObrirFractal()
{
// TODO: Agregue aquí su código de controlador de comandos
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
	CFileDialog openMunt(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openMunt.DoModal() != IDOK)
		return;                 // stay with old data file
	else nom = openMunt.GetPathName();

// Entorn VGI: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn VGI: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// Entorn VGI: Variable de tipus char *nomfitx conté el nom del fitxer seleccionat


// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


// OnArchivoObrirFitxerObj: Obrir fitxer en format gràfic OBJ
void CEntornVGIView::OnArxiuObrirFitxerObj()
{
// TODO: Agregue aquí su código de controlador de comandos
//if (ObOBJ != NULL) delete ObOBJ;

	objecte = OBJOBJ;	textura = true;

// Entorn VGI: Obrir diàleg de lectura de fitxer
	CFileDialog openOBJ(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("OBJ Files(*.obj)|*.obj|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
	else nom = openOBJ.GetPathName();

// Entorn VGI: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn VGI: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJ,false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT,true);	// Carregar objecte OBJ AMB textura

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Desactivem contexte OpenGL

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

// Obrir fitxer en format gràfic 3DS
void CEntornVGIView::OnArxiuObrirFitxer3ds()
{
// TODO: Agregue aquí su código de controlador de comandos
	//if(Ob3DS!=NULL) delete Ob3DS;

	objecte = OBJ3DS;	textura = true;

// Entorn VGI: Obrir diàleg de lectura de fitxer
	CFileDialog open3DS(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("3DS Files(*.3ds)|*.3ds|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (open3DS.DoModal() != IDOK)	return;  // stay with old data file
	else nom = open3DS.GetPathName();

// Entorn VGI: Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

// Entorn VGI: Conversió de la variable CString nom a la variable char *nomfitx, 
//		compatible amb  les funcions de càrrega de fitxers fractals
	char *nomfitx = CString2Char(nom);

// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); // Activem contexte OpenGL

	if (Ob3DS == NULL) Ob3DS = new Obj_3DS;
	Ob3DS->EliminarMemoria();
	Ob3DS->Inicialitzar();
	Ob3DS->Carregar3DS(nomfitx);

// Precompilació de dos objectes nous: OBJECTE3DS: Objecte 3DS sense textures i OBJECTE3DST amb textures,
	Ob3DS->Dibuixa3DS(false, OBJECTE3DS, false);
	Ob3DS->Dibuixa3DS(false, OBJECTE3DST, true);

	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL); // Desactivem contexte OpenGL

//  Entorn VGI: Modificar R per centrar Vista amb mida de l'objecte
	mida = sqrtf(3.0) * 10;
	OPV.R = 0.5*mida / sin(30 * pi / 180) + p_near;

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}


/* ------------------------------------------------------------------------- */
/*					2. VEURE: ToolBar, StatusBar, Pantalla Completa         */
/* ------------------------------------------------------------------------- */





/* ------------------------------------------------------------------------- */
/*					10. SHADERS												 */
/* ------------------------------------------------------------------------- */

// SHADERS: Desactivació de tots els shaders
void CEntornVGIView::OnShadersSense()
{
// TODO: Agregue aquí su código de controlador de comandos
	shader_menu = CAP_SHADER;
	releaseAllShaders();

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateShadersSense(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == CAP) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Iluminació Gouraud amb Shaders
void CEntornVGIView::OnShadersGouraud()
{
// TODO: Agregue aquí su código de controlador de comandos
	bool apagat = false;
	if (sw_shader) releaseAllShaders();
	else sw_shader = true;
	shader_menu = GOURAUD_SHADER;

// Entorn VGI: Activació del contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	shader_program = initializeShaders(_T("./shaders/gouraud_shdrML"));

	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
// Pas màscara reflectivitat
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
//Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL[0].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), llumGL[1].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), llumGL[2].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), llumGL[3].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), llumGL[4].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[5]"), llumGL[5].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[6]"), llumGL[6].encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[7]"), llumGL[7].encesa);

// Entorn VGI: Desactivació del contexte OpenGL. Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateShadersGouraud(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == GOURAUD_SHADER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}





/* ------------------------------------------------------------------------- */
/*   HURAKAN										                          */
/* ------------------------------------------------------------------------- */


void CEntornVGIView::OnHurakanBrazos()
{	
	//OPTIONS
	projeccio = PERSPECT;
	mobil = true;			
	objecte = OBJOBJ;
	textura = true;

	//ILUMINACIO PLANA
	ilumina = PLANA;
	test_vis = false;		oculta = true;


	//PATH OBJ
	char* nomfitx = CString2Char(PATH_ARM);
	char* nomTexture = CString2Char(PATH_TEXTURE_ARM);

	// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomTexture);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));


	OnShadersGouraud();


	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	//ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateHurakanBrazos(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == OBJOBJ) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnHurakanBase()
{

	//OPTIONS
	projeccio = PERSPECT;
	mobil = true;			
	objecte = BASE;
	textura = true;
	ilumina = PLANA;
	test_vis = false;		
	oculta = true;


	//PATH OBJ
	char* nomfitx = CString2Char(PATH_BASE);

	char* nomTexture = CString2Char(PATH_TEXTURE_BASE);

	// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomTexture);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));


	OnShadersGouraud();


	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	//ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateHurakanBase(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == BASE) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnHurakanAsientos()
{
	//OPTIONS
	projeccio = PERSPECT;
	mobil = true;
	objecte = SEIENTS;
	textura = true;
	ilumina = PLANA;
	test_vis = false;		
	oculta = true;


	//PATH OBJ
	char* nomfitx = CString2Char(PATH_ASIENTO);
	char* nomTexture = CString2Char(PATH_TEXTURE_SEIENTS);

	// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomTexture);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));


	OnShadersGouraud();


	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	//ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateHurakanAsientos(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == SEIENTS) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnHurakanSkydome()
{



	//PERSPECTIVA
	projeccio = PERSPECT;
	mobil = true;			//zzoom = true;

	//OBJ
	//objecte = OBJOBJ;	
	objecte = SKYDOME;

	//textura = true;

	//ILUMINACIO PLANA
	ilumina = PLANA;
	test_vis = false;		oculta = true;


	//PATH OBJ
	char* nomfitx = CString2Char(PATH_SKYDOME);

	char* nomTexture = CString2Char(PATH_TEXTURE_SKYDOME);

	// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomTexture);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));


	OnShadersGouraud();


	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	//ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateHurakanSkydome(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == SKYDOME) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}
void CEntornVGIView::OnHurakanFloor()
{



	//PERSPECTIVA
	projeccio = PERSPECT;
	mobil = true;			//zzoom = true;

	//OBJ
	//objecte = OBJOBJ;	
	objecte = FLOOR;

	//textura = true;

	//ILUMINACIO PLANA
	ilumina = PLANA;
	test_vis = false;		oculta = true;


	//PATH OBJ
	char* nomfitx = CString2Char(PATH_FLOOR);

	char* nomTexture = CString2Char(PATH_TEXTURE_FLOOR);

	// Entorn VGI: Activació el contexte OpenGL
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	texturesID[0] = loadIMA_SOIL(nomTexture);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));


	OnShadersGouraud();


	// i carreguem
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	// Activem contexte OpenGL

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	//ObOBJ->LoadModel(nomfitx, OBJECTEOBJ, false);	// Carregar objecte OBJ SENSE textura
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJT, true);	// Carregar objecte OBJ AMB textura

	// Crida a OnPaint() per redibuixar l'escena
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateHurakanFloor(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == FLOOR) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



void CEntornVGIView::OnHurakan()
{	

	// TODO: Agregue aquí su código de controlador de comandos
	char* nom_braç = CString2Char(PATH_ARM);
	char* nom_base = CString2Char(PATH_BASE);
	char* nom_asiento = CString2Char(PATH_ASIENTO);
	char* nom_floor = CString2Char(PATH_FLOOR);
	char* nom_skydome = CString2Char(PATH_SKYDOME);


	char* nomTextureArm = CString2Char(PATH_TEXTURE_ARM);
	char* nomTextureBase = CString2Char(PATH_TEXTURE_BASE);
	char* nomTextureSeient = CString2Char(PATH_TEXTURE_SEIENTS);
	char* nomTextureSkydome = CString2Char(PATH_TEXTURE_SKYDOME);
	char* nomTextureFloor = CString2Char(PATH_TEXTURE_FLOOR);


	objecte = HURAKAN;
	projeccio = PERSPECT;
	mobil = true;
	ilumina = PLANA;
	oculta = true;
	if (textura) {
		textura = false;
	}
	else {
		textura = true;
	}
	
	demo_on = true;
	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);


	texturesID[OBJECTEBRAC] = loadIMA_SOIL(nomTextureArm);
	texturesID[OBJECTEBASE] = loadIMA_SOIL(nomTextureBase);
	texturesID[OBJECTESEIENT] = loadIMA_SOIL(nomTextureSeient);
	texturesID[OBJECTESKYDOME] = loadIMA_SOIL(nomTextureSkydome);
	texturesID[OBJECTEFLOOR] = loadIMA_SOIL(nomTextureFloor);


	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	ObOBJ->LoadModel(nom_braç, OBJECTEBRAC, true);
	ObOBJ->LoadModel(nom_base, OBJECTEBASE, true);
	ObOBJ->LoadModel(nom_asiento, OBJECTESEIENT, true);
	ObOBJ->LoadModel(nom_floor, OBJECTEFLOOR, true);
	ObOBJ->LoadModel(nom_skydome, OBJECTESKYDOME, true);

	//	Pas de textura al shader
	if (shader_menu != CAP_SHADER) glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
	OnShadersGouraud();

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);


	CDC* pDC = GetDC();
	//m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);

	

}

void CEntornVGIView::OnUpdateHurakan(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == HURAKAN) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



/* ------------------------------------------------------------------------- */
/*   RECURSOS DE BOTONS (ToolBar) DE L'APLICACIO                             */
/* ------------------------------------------------------------------------- */
// Incloure funcions de tractament de botons


// ---------------- Entorn VGI: Funcions locals a EntornVGIView.cpp

// Log2: Càlcul del log base 2 de num
int CEntornVGIView::Log2(int num)
{
	int tlog;

	if (num >= 8192) tlog = 13;
	else if (num >= 4096) tlog = 12;
	else if (num >= 2048) tlog = 11;
	else if (num >= 1024) tlog = 10;
	else if (num >= 512) tlog = 9;
	else if (num >= 256) tlog = 8;
	else if (num >= 128) tlog = 7;
	else if (num >= 64) tlog = 6;
	else if (num >= 32) tlog = 5;
	else if (num >= 16) tlog = 4;
	else if (num >= 8) tlog = 3;
	else if (num >= 4) tlog = 2;
	else if (num >= 2) tlog = 1;
	else tlog = 0;

	return tlog;
}


// CString2char: Funció de conversió de variable CString a char * per a noms de fitxers 
char * CEntornVGIView::CString2Char(CString entrada)
{
//char * par_sortida=" ";
// Variable de tipus CString 'entrada' conté un string tipus CString
//-------------------------------------------------------------------------------------
// Conversió de la variable CString entrada a la variable char *sortida, 
//	compatible amb la funció Carregar3DS, de càrrega de fitxers 3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;

// Conversió variable w_char --> char *
//	char *nomf="  ";
//	wcstombs(nomf,strFileName.GetBuffer(3),90);
//	char *nomf = reinterpret_cast<char *> (nom.GetBuffer(3));

	size_t origsize = wcslen(entrada.GetBuffer(3)) + 1;
	size_t convertedChars = 0;

// Use a multibyte string to append the type of string
// to the new string before displaying the result.
	char strConcat[] = " (char *)";
	size_t strConcatsize = (strlen(strConcat) + 1) * 2;

// Allocate two bytes in the multibyte output string for every wide
// character in the input string (including a wide character
// null). Because a multibyte character can be one or two bytes,
// you should allot two bytes for each character. Having extra
// space for the new string is not an error, but having
// insufficient space is a potential security problem.
	const size_t newsize = origsize * 2;
// The new string will contain a converted copy of the original
// string plus the type of string appended to it.
//	char *nomfitx = new char[newsize + strConcatsize];
	char *par_sortida = new char[newsize + strConcatsize];

// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, par_sortida, newsize, entrada.GetBuffer(3), _TRUNCATE);
// append the type of string to the new string.
//----------------------------------------------------------------------------------

// Variable de tipus char *nomfitx conté el nom del fitxer seleccionat
	return par_sortida;
}


// Refl_MaterialOff: Desactivar Reflexió de Material
void CEntornVGIView::Refl_MaterialOff()
{
	sw_material_old[0] = sw_material[0];	sw_material[0] = false;
	sw_material_old[1] = sw_material[1];	sw_material[1] = false;
	sw_material_old[2] = sw_material[2];	sw_material[2] = false;
	sw_material_old[3] = sw_material[3];	sw_material[3] = false;
}

// Refl_MaterialOn: Activar Reflexió de Material
void CEntornVGIView::Refl_MaterialOn()
{
	sw_material[0] = sw_material_old[0];
	sw_material[1] = sw_material_old[1];
	sw_material[2] = sw_material_old[2];
	sw_material[3] = sw_material_old[3];
}


// -------------------- FUNCIONS CORBES SPLINE i BEZIER

// llegir_ptsC: Llegir punts de control de corba (spline o Bezier) d'un fitxer .crv. 
//				Retorna el nombre de punts llegits en el fitxer.
//int llegir_pts(CString nomf)
int CEntornVGIView::llegir_ptsC(char *nomf)
{
	int i, j;
	FILE *fd;

	// Inicialitzar vector punts de control de la corba spline
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{
		PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	//	ifstream f("altinicials.dat",ios::in);
	//    f>>i; f>>j;
	if ((fd = fopen(nomf, "rt")) == NULL)
	{
		LPCWSTR texte1 = reinterpret_cast<LPCWSTR> ("ERROR:");
		LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("File .crv was not opened");
		//MessageBox(NULL, texte1, texte2, MB_OK);
		MessageBox(texte1, texte2, MB_OK);
		//AfxMessageBox("file was not opened");
		return false;
	}
	fscanf(fd, "%d \n", &i);
	if (i == 0) return false;
	else {
		for (j = 0; j < i; j = j++)
		{	//fscanf(fd, "%f", &corbaSpline[j].x);
			//fscanf(fd, "%f", &corbaSpline[j].y);
			//fscanf(fd, "%f \n", &corbaSpline[j].z);
			fscanf(fd, "%f %f %f \n", &PC_t[j].x, &PC_t[j].y, &PC_t[j].z);

		}
	}
	fclose(fd);

	return i;
}


void CEntornVGIView::OnCiclosPrueba1()
{
	// TODO: Agregue aquí su código de controlador de comandos
	// TODO: Agregue aquí su código de controlador de comandos
	
	if (objecte == HURAKAN) {
		//PERSPECTIVA
		

		if (d1.demo_on) {
			d1.reset_demo();
			d1.start_demo("brac_moves.txt","seient_moves.txt");
		}
		else {

			//OBJ
			//objecte = OBJOBJ;	

			d1.demo_on = true;
			textura = true;
			d1.mode = 1;
			d1.start_demo("brac_moves.txt", "seient_moves.txt");
			anima = true;
		}

		//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
		//	Canviar l'escala per a centrar la vista (Ortogràfica)

		// Crida a OnPaint() per redibuixar l'escena
		InvalidateRect(NULL, false);
	}
}


void CEntornVGIView::OnUpdateCiclosPrueba1(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (d1.demo_on) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornVGIView::OnCiclosPruebagrabada()
{
	// TODO: Agregue aquí su código de controlador de comandos
	if (objecte == HURAKAN) {
		//PERSPECTIVA


		if (d1.demo_on) {
			d1.reset_demo();
			d1.start_demo("grabacio_brac.txt","grabacio_seients.txt");
		}
		else {

			//OBJ
			//objecte = OBJOBJ;	

			d1.demo_on = true;
			textura = true;
			d1.mode = 1;
			d1.start_demo("grabacio_brac.txt", "grabacio_seients.txt");
			anima = true;
		}

		//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
		//	Canviar l'escala per a centrar la vista (Ortogràfica)

		// Crida a OnPaint() per redibuixar l'escena
		InvalidateRect(NULL, false);
	}
}


void CEntornVGIView::OnUpdateCiclosPruebagrabada(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (d1.demo_on) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



void CEntornVGIView::OnCamarasExteriorFrontal()
{
	// TODO: Agregue aquí su código de controlador de comandos
	cam = EXTERIOR_FRONTAL;
	mobil = true;
	OPV.alfa = 0;
	OPV.beta = 270;
	zzoom = false;
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateCamarasExteriorFrontal(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (cam == EXTERIOR_FRONTAL) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CEntornVGIView::OnCamarasTemple()
{
	// TODO: Agregue aquí su código de controlador de comandos
	cam = TEMPLE_CAM;
	mobil = true;
	zzoom = true;
	OPV.alfa = 0;
	OPV.beta = 90;
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateCamarasTemple(CCmdUI* pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (cam == TEMPLE_CAM) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnCamarasSeients()
{
	// TODO: Agregue aquí su código de controlador de comandos
	cam = CAM_ASIENTOS;
	mobil = true;
	zzoom = false;
	OPV.alfa = 0;
	OPV.beta = 90;
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateCamarasSeients(CCmdUI* pCmdUI)
{

	if (cam == CAM_ASIENTOS) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnCamarasSplit()
{
	// TODO: Agregue aquí su código de controlador de comandos
	cam = SPLIT_CAM;
	mobil = true;
	zzoom = false;
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateCamarasSplit(CCmdUI* pCmdUI)
{

	if (cam == SPLIT_CAM) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CEntornVGIView::OnCamarasDefault()
{
	// TODO: Agregue aquí su código de controlador de comandos
	cam = DEFAULT_CAM;
	mobil = true;
	zzoom = false;
	OPV.alfa = 10;
	OPV.beta = 90;
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateCamarasDefault(CCmdUI* pCmdUI)
{

	if (cam == DEFAULT_CAM) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



