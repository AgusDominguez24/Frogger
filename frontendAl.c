#include <stdio.h>
#include "frontendAl.h"

/* VARIABLES DE ALLEGRO */
ALLEGRO_DISPLAY* ventana; // puntero global: apunta a nuestra ventana
ALLEGRO_EVENT evento;
ALLEGRO_EVENT_QUEUE* colaEventos;
ALLEGRO_BITMAP* icon;
ALLEGRO_COLOR transparente;
ALLEGRO_COLOR blanco;
ALLEGRO_COLOR azul;
ALLEGRO_COLOR verde;
ALLEGRO_COLOR verdeoscuro;
ALLEGRO_COLOR negro;
ALLEGRO_BITMAP* logo;
ALLEGRO_BITMAP* pasto;
ALLEGRO_BITMAP* pastoMenu;
ALLEGRO_BITMAP* pastoGame;
ALLEGRO_BITMAP* agua;
ALLEGRO_BITMAP* aguaMenu;
ALLEGRO_BITMAP* calle;
ALLEGRO_BITMAP* nada;
ALLEGRO_BITMAP* vidas;
ALLEGRO_BITMAP* muertes;
ALLEGRO_BITMAP* auto1;
ALLEGRO_BITMAP* auto2;
ALLEGRO_BITMAP* auto3;
ALLEGRO_BITMAP* auto4;
ALLEGRO_BITMAP* autobus;
ALLEGRO_BITMAP* troncos3;
ALLEGRO_BITMAP* troncos5;
ALLEGRO_BITMAP* tortugas2;
ALLEGRO_BITMAP* tortugas3;
ALLEGRO_FONT* gameria[7];
ALLEGRO_BITMAP* skin[6];

unsigned int skinElegida = RANA;

position_t mouse = {0,0};

int botonAl;


/* PROTOTIPOS ESTATICAS */

// En el caso del menú, imprime los botones y el fondo
static void menup(void);

// Imprime los botones del menú, con un sombreado oscuro de fondo
static void botonesMenu(void);

// Inicia el juego
static void newgame(void);

static void imprimirAgua(void);

// Imprime los botones del menú, con un sombreado oscuro de fondo
static void botonesMenu(void);

// En el caso de seleccionar el menú de skins, Imprime el fondo y los botones correspondientes
static void skins(void);

// Botones del menú de skins
static void botonesSkins(void);

// Menú de pausa
static void pausar(void);

// Función que imprime los botones al apretar pause (esc)
static void botonesPausa(void);

// Función para la animación de muerte de la rana
static void muere(void);

// Función para cuando el juego termina
static void gameover(void);

// Imprime los botones al estar en la sección gameover 
static void botonesGameover(void);

// Función para crear el fondo, indicando los respectivos bitmaps
static void imprimirFondo(unsigned int arregloFondo[16]);

// imprime los objetos, autos, autobuses, troncos y tortugas
static void imprimirObjetos(void);

// Imprime las vidas (corazones) en la posición correspondiente
static void imprimirVidas(void);

// imprime las 5 metas
static void imprimirMetas(void);

// Imprime el jugador, en cada situacion posible (calle, tronco, o tortugar)
static void imprimirPlayer(int x, int y, int facing);

// Carga las imágenes
static int path(void);



/* FUNCIONES */
void renderizado(void) // se encarga de filtar y llamar a las funciones que imprimen
{
	switch(pagina)
	{
	case MENUP:
	
		menup();
		
		break;
		
	case NEWGAME:
		
		newgame();
		break;
		
	case SKINS:
	
		skins();
		break;
		
	case PAUSA:
	
		pausar();
		break;
		
	case MUERTE:
	
		muere();
		break;
		
	case GAMEOVER:
	
		gameover();
		break;
		
	}
	al_flip_display();	// Muestra
	return;
}


static void menup(void)	// En el caso del menú, imprime los botones y el fondo
{
	imprimirFondo(arregloFondo);
	botonesMenu();
	return;
}

static void botonesMenu(void)	// Imprime los botones del menú, con un sombreado oscuro de fondo
{
	al_draw_text(gameria[0], verdeoscuro, fontleftside(gameria[0], "FROGGER") - 4, 154, ALLEGRO_ALIGN_LEFT, "FROGGER");
	al_draw_text(gameria[0], verde, fontleftside(gameria[0], "FROGGER"), 150, ALLEGRO_ALIGN_LEFT, "FROGGER");
	
	if (botonAl == NEWGAME)
	{
		al_draw_text(gameria[6], verde, fontleftside(gameria[6], "NEW GAME") - 2, 302, ALLEGRO_ALIGN_LEFT, "NEW GAME");
		al_draw_text(gameria[6], verdeoscuro, fontleftside(gameria[6], "NEW GAME"), 300, ALLEGRO_ALIGN_LEFT, "NEW GAME");
	}
	else
	{
		al_draw_text(gameria[5], verdeoscuro, fontleftside(gameria[5], "NEW GAME") - 2, 302, ALLEGRO_ALIGN_LEFT, "NEW GAME");
		al_draw_text(gameria[5], verde, fontleftside(gameria[5], "NEW GAME"), 300, ALLEGRO_ALIGN_LEFT, "NEW GAME");
	}
	if (botonAl == SKINS)
	{
		al_draw_text(gameria[6], verde, fontleftside(gameria[6], "SKINS") - 2, 372, ALLEGRO_ALIGN_LEFT, "SKINS");
		al_draw_text(gameria[6], verdeoscuro, fontleftside(gameria[6], "SKINS"), 370, ALLEGRO_ALIGN_LEFT, "SKINS");
	}
	else
	{
		al_draw_text(gameria[5], verdeoscuro, fontleftside(gameria[5], "SKINS") - 2, 372, ALLEGRO_ALIGN_LEFT, "SKINS");
		al_draw_text(gameria[5], verde, fontleftside(gameria[5], "SKINS"), 370, ALLEGRO_ALIGN_LEFT, "SKINS");
	}

	if (botonAl == EXIT)
	{
		al_draw_text(gameria[6], verde, fontleftside(gameria[6], "EXIT") - 2, 442, ALLEGRO_ALIGN_LEFT, "EXIT");
		al_draw_text(gameria[6], verdeoscuro, fontleftside(gameria[6], "EXIT"), 440, ALLEGRO_ALIGN_LEFT, "EXIT");
	}
	else
	{
		al_draw_text(gameria[5], verdeoscuro, fontleftside(gameria[5], "EXIT") - 2, 442, ALLEGRO_ALIGN_LEFT, "EXIT");
		al_draw_text(gameria[5], verde, fontleftside(gameria[5], "EXIT"), 440, ALLEGRO_ALIGN_LEFT, "EXIT");
	}

	return;
}


static void imprimirAgua(void) // animacion del agua
{
	static int curFrame = 0;
	if (curFrame < 7)
	{
		curFrame++;
	}
	else
	{
		curFrame = 0;
	}
	
	al_draw_bitmap_region(agua, 0, curFrame * 210, 560, 210, 0, 2*SIZEOFSKIN, 0);	// Imprime el bitmap correspondiente 
	
	usleep(70000);
	
	return;
}

static void imprimirAguaMenu(void) // animacion de otra agua, depende de la pagina
{
	static int curFrame = 0;
	if (curFrame < 7)
	{
		curFrame++;
	}
	else
	{
		curFrame = 0;
	}
	
	al_draw_bitmap_region(aguaMenu, 0, curFrame * 560, 560, 560, 0, 1*SIZEOFSKIN, 0);	// Imprime el bitmap correspondiente 
	
	usleep(70000);
	
	return;
}


static void newgame(void) //imprime todo lo necesario para el juego
{
	imprimirFondo(arregloFondo);	// Imprime el fondo
	imprimirObjetos(); // imprime los objetos del juego
	imprimirVidas(); // imprime las vidas
	imprimirMetas(); // imprime las metas alcanzadas
	imprimirPlayer(player.x, player.y, player.facing); //imprime el jugador
	
	al_draw_textf(gameria[2], blanco, SIZEOFSKIN / 2, 1, ALLEGRO_ALIGN_LEFT, "SCORE %d", player.puntaje);	// Imprime el Score actual
	al_draw_textf(gameria[2], blanco, SIZEOFSKIN*11 + 15, SIZEOFSKIN*15, ALLEGRO_ALIGN_LEFT, "TIME %d", timer);	// Imprime el Timer
	
	al_draw_textf(gameria[2], blanco, SIZEOFSKIN*5, SIZEOFSKIN*15, ALLEGRO_ALIGN_LEFT, "LEVEL %d", nivel); // Imprime el nivel actual
	
	// Imprime el High Score
	if (high_score >= 0 && player.puntaje <= 99)	// Imprime el high socre, analizando la cantidad de dígitos del mismo para alinearlo 
	{	
		al_draw_textf(gameria[2], blanco, 550 - al_get_text_width(gameria[2], "HIGH SCORE XX"), 1, ALLEGRO_ALIGN_LEFT, "HIGH SCORE %d", high_score);
	}
	else if (high_score >= 100 && player.puntaje <= 999)
	{
		al_draw_textf(gameria[2], blanco, 550 - al_get_text_width(gameria[2], "HIGH SCORE XXX"), 1, ALLEGRO_ALIGN_LEFT, "HIGH SCORE %d", high_score);
	}
	
	
	return;
}

static void skins(void)	// En el caso de seleccionar el menú de skins, imprime el fondo y los botones correspondientes
{
	imprimirFondo(arregloFondo);
	botonesSkins();
	return;
}

static void botonesSkins(void)	// Botones del menú de skins
{
	al_draw_text(gameria[3], verdeoscuro, fontleftside(gameria[3], "SKINS") - 4, 154, ALLEGRO_ALIGN_LEFT, "SKINS");	// Crea el encabezado
	al_draw_text(gameria[3], verde, fontleftside(gameria[3], "SKINS"), 150, ALLEGRO_ALIGN_LEFT, "SKINS");	// Con su sombreado correspondiente
	
	if (botonAl == RANA)	// Imprime el botón de la rana, en el caso de estar seleccionada
	{
		al_draw_filled_rectangle(105,305,155,355,verde); // Crea el rectángulo
		al_draw_filled_rectangle(110,310,150,350,verdeoscuro); // Crea el rectángulo oscuro de fondo
		al_draw_bitmap_region(skin[RANA-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 112,311, 0);	// Coloca la imágen de la skin en cuestión 
	}
	else // imprime el botón de la rana, en el caso de no estar seleccionada
	{
		al_draw_filled_rectangle(105,305,155,355,verdeoscuro); //RANA
		al_draw_filled_rectangle(110,310,150,350,verde); //RANA
		al_draw_bitmap_region(skin[RANA-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 112,311, 0);
	}

	if (botonAl == GATO)
	{
		al_draw_filled_rectangle(165,305,215,355,verde); //GATO
		al_draw_filled_rectangle(170,310,210,350,verdeoscuro); //GATO
		al_draw_bitmap_region(skin[GATO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 174,314, 0);
	}
	else
	{
		al_draw_filled_rectangle(165,305,215,355,verdeoscuro); //GATO
		al_draw_filled_rectangle(170,310,210,350,verde); //GATO
		al_draw_bitmap_region(skin[GATO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 174,314, 0);
	}

	if (botonAl == PATO)
	{
		al_draw_filled_rectangle(225,305,275,355,verde); //PATO
		al_draw_filled_rectangle(230,310,270,350,verdeoscuro); //PATO
		al_draw_bitmap_region(skin[PATO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 233,313, 0);
	}
	else
	{
		al_draw_filled_rectangle(225,305,275,355,verdeoscuro); //PATO
		al_draw_filled_rectangle(230,310,270,350,verde); //PATO
		al_draw_bitmap_region(skin[PATO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 233,313, 0);
	}
	
	if (botonAl == PERRO)
	{
		al_draw_filled_rectangle(285,305,335,355,verde); //PERRO
		al_draw_filled_rectangle(290,310,330,350,verdeoscuro); //PERRO
		al_draw_bitmap_region(skin[PERRO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 292,313, 0);
	}
	else
	{
		al_draw_filled_rectangle(285,305,335,355,verdeoscuro); //PERRO
		al_draw_filled_rectangle(290,310,330,350,verde); //PERRO
		al_draw_bitmap_region(skin[PERRO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 292,313, 0);
	}

	if (botonAl == CHANCHO)
	{
		al_draw_filled_rectangle(345,305,395,355,verde); //CHANCHO
		al_draw_filled_rectangle(350,310,390,350,verdeoscuro); //CHANCHO
		al_draw_bitmap_region(skin[CHANCHO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 352,313, 0);
	}
	else
	{
		al_draw_filled_rectangle(345,305,395,355,verdeoscuro); //CHANCHO
		al_draw_filled_rectangle(350,310,390,350,verde); //CHANCHO
		al_draw_bitmap_region(skin[CHANCHO-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 352,313, 0);
	}

	if (botonAl == AVESTRUZ)
	{
		al_draw_filled_rectangle(405,305,455,355,verde); //AVESTRUZ
		al_draw_filled_rectangle(410,310,450,350,verdeoscuro); //AVESTRUZ
		al_draw_bitmap_region(skin[AVESTRUZ-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 413,313, 0);
	}
	else
	{
		al_draw_filled_rectangle(405,305,455,355,verdeoscuro); //AVESTRUZ
		al_draw_filled_rectangle(410,310,450,350,verde); //AVESTRUZ
		al_draw_bitmap_region(skin[AVESTRUZ-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, 413,313, 0);
	}

	if (botonAl == BACK)	// Imprime el boton back para regreasar al menú principal
	{
		al_draw_text(gameria[2], verde, fontleftside(gameria[2], "BACK") - 2, 502, ALLEGRO_ALIGN_LEFT, "BACK");
		al_draw_text(gameria[2], verdeoscuro, fontleftside(gameria[2], "BACK"), 500, ALLEGRO_ALIGN_LEFT, "BACK");
	}
	else
	{
		al_draw_text(gameria[1], verdeoscuro, fontleftside(gameria[1], "BACK") - 2, 502, ALLEGRO_ALIGN_LEFT, "BACK");
		al_draw_text(gameria[1], verde, fontleftside(gameria[1], "BACK"), 500, ALLEGRO_ALIGN_LEFT, "BACK");
	}

	return;
}

static void pausar(void) // Menú de pausa
{
	al_draw_filled_rectangle(0,0,560,560, azul); // crea el rectángulo para el menu
	botonesPausa();	// Función que crea los botones
	return;
}

static void botonesPausa(void)	// Función que crea los botones
{
	
	al_draw_text(gameria[3], verdeoscuro, fontleftside(gameria[3], "PAUSA") - 2, 132, ALLEGRO_ALIGN_LEFT, "PAUSA");	// Encabezado de PAUSA con su sombra
	al_draw_text(gameria[3], verde, fontleftside(gameria[3], "PAUSA"), 130, ALLEGRO_ALIGN_LEFT, "PAUSA");

	if (botonAl == REANUDAR)	// Si el botón reanudar está seleccionado 
	{
		al_draw_text(gameria[2], verde, fontleftside(gameria[2], "REANUDAR") - 2, 250, ALLEGRO_ALIGN_LEFT, "REANUDAR");	// Crea el contorno verde 
		al_draw_text(gameria[2], verdeoscuro, fontleftside(gameria[2], "REANUDAR"), 252, ALLEGRO_ALIGN_LEFT, "REANUDAR"); // Crea el botón verde oscuro
	}
	else  // Si el botón no está seleccionado 
	{
		al_draw_text(gameria[1], verdeoscuro, fontleftside(gameria[1], "REANUDAR") - 2, 252, ALLEGRO_ALIGN_LEFT, "REANUDAR");	// Crea el botón en verde
		al_draw_text(gameria[1], verde, fontleftside(gameria[1], "REANUDAR"), 250, ALLEGRO_ALIGN_LEFT, "REANUDAR");	// Con su respectivo contorno verde oscuro 
	}

	if (botonAl == ENDGAME)	// Mismo caso pero con el boton de endgame
	{
		al_draw_text(gameria[2], verde, fontleftside(gameria[2], "END GAME") - 2, 300, ALLEGRO_ALIGN_LEFT, "END GAME");
		al_draw_text(gameria[2], verdeoscuro, fontleftside(gameria[2], "END GAME"), 302, ALLEGRO_ALIGN_LEFT, "END GAME");
	}
	else
	{
		al_draw_text(gameria[1], verdeoscuro, fontleftside(gameria[1], "END GAME") - 2, 302, ALLEGRO_ALIGN_LEFT, "END GAME");
		al_draw_text(gameria[1], verde, fontleftside(gameria[1], "END GAME"), 300, ALLEGRO_ALIGN_LEFT, "END GAME");
	}

	return;
}

static void muere(void)	// Función para la animación de muerte de la rana
{
	int frameDelay = 5;
	static int curFrame = 0;
	static int delayCounter = 0;
	if (delayCounter < frameDelay)
	{
		delayCounter++;
	}
	else
	{
		if (curFrame < 7)
		{
			curFrame++;
		}
		else
		{
			pagina = NEWGAME;
			player.x = POSINIX;
			player.y = POSINIY;
			curFrame = 0;
		}
		delayCounter = 0;
	}
	
	al_draw_bitmap_region(muertes, curFrame * SIZEOFSKIN, 0, SIZEOFSKIN, SIZEOFSKIN, player.x * SIZEOFSKIN, player.y * SIZEOFSKIN, 0);	// Imprime el bitmap correspondiente 
	usleep(3000);
	return;
}

static void gameover(void)	// Función para cuando el juego termina
{
	al_draw_filled_rectangle(0,0,560,560, azul); // Imprime un rectángulo 
	botonesGameover();	// Imprime los botones
	return;
}

static void botonesGameover(void)	// Imprime los botones
{
	al_draw_text(gameria[3], verdeoscuro, fontleftside(gameria[3], "GAMEOVER") - 2, 132, ALLEGRO_ALIGN_LEFT, "GAMEOVER");	// Encabezado de GAMEOVER con su sombra
	al_draw_text(gameria[3], verde, fontleftside(gameria[3], "GAMEOVER"), 130, ALLEGRO_ALIGN_LEFT, "GAMEOVER");

	if (player.puntaje >= 0 && player.puntaje <= 99)	// Imprime el puntaje logrado, analiza la cantidad de dígitos del mismo para alinealrlo 
	{
		al_draw_textf(gameria[2], verde, 10+ fontleftside(gameria[2], "SCORE XX"), SIZEOFSKIN*7, ALLEGRO_ALIGN_LEFT, "SCORE %d", player.puntaje);
	}
	else if (player.puntaje >= 100 && player.puntaje <= 999)
	{
		al_draw_textf(gameria[2], verde, 10+ fontleftside(gameria[2], "SCORE XXX"), SIZEOFSKIN*7, ALLEGRO_ALIGN_LEFT, "SCORE %d", player.puntaje);
	}
	

	if (high_score >= 0 && player.puntaje <= 99)	// Imprime el high socre, analizando la cantidad de dígitos del mismo para alinearlo 
	{
		al_draw_textf(gameria[2], verde, fontleftside(gameria[2], "HIGH SCORE XX"), SIZEOFSKIN*9, ALLEGRO_ALIGN_LEFT, "HIGH SCORE %d", high_score);
	}
	else if (high_score >= 100 && player.puntaje <= 999)
	{
		al_draw_textf(gameria[2], verde, fontleftside(gameria[2], "HIGH SCORE XXX"), SIZEOFSKIN*9, ALLEGRO_ALIGN_LEFT, "HIGH SCORE %d", high_score);
	}
	

	if (botonAl == QUIT)	// Boton de quit
	{
		al_draw_text(gameria[2], verde, fontleftside(gameria[2], "QUIT") - 2, 400, ALLEGRO_ALIGN_LEFT, "QUIT");
		al_draw_text(gameria[2], verdeoscuro, fontleftside(gameria[2], "QUIT"), 402, ALLEGRO_ALIGN_LEFT, "QUIT");
	}
	else
	{
		al_draw_text(gameria[1], verdeoscuro, fontleftside(gameria[1], "QUIT") - 2, 402, ALLEGRO_ALIGN_LEFT, "QUIT");
		al_draw_text(gameria[1], verde, fontleftside(gameria[1], "QUIT"), 400, ALLEGRO_ALIGN_LEFT, "QUIT");
	}

	return;
}

static void imprimirFondo(unsigned int arregloFondo[16]) // Función para imprimir el fondo, indicando los respectivos bitmaps
{
	int i; // variables auxiliares para el for y para las animaciones
	
	for (i = MAXCOL-1 ; i >= 0 ; i--)
	{
		switch(arregloFondo[i])
		{

		case PASTO:	// Si se trata del pasto, imprime la imagen en cuestión 
			al_draw_bitmap(pasto, 0, SIZEOFSKIN * i, 0);
			break;

		case PASTOMENU: // Si se trata del pasto en el menú, imprime la imagen en cuestión
			al_draw_bitmap(pastoMenu, 0, SIZEOFSKIN * i, 0);
			break;

		case PASTOGAME: // Si se trata del pasto del juego, imprime la imagen en cuestión 
			al_draw_bitmap(pastoGame, 0, SIZEOFSKIN * i, 0);
			break;

		case AGUA: // Imprime el agua

			imprimirAgua();
			break;

		case AGUAMENU:

			imprimirAguaMenu();
			break;

		case CALLE:
			al_draw_bitmap(calle, 0, SIZEOFSKIN * i, 0);
			break;

		case NADA:
			al_draw_bitmap(nada, 0, SIZEOFSKIN * i, 0);
			break;

		default:
			break;
		}

	}
	return;
}

static void imprimirObjetos(void)	// Imprime los objetos, autos, autobuses, troncos y tortugas
{
	int i;
	for (i = 0; i < OBJETOS; i++)	// Recorre los disintos objetos, e imprime cada uno en su fila correspondiente
	{
		switch (objetos[i].tipo)
		{

		case AUTO1:
			al_draw_bitmap(auto1, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case AUTO2:
			al_draw_bitmap(auto2, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case AUTO3:
			al_draw_bitmap(auto3, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case AUTO4:
			al_draw_bitmap(auto4, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case TRONCOS3:
			al_draw_bitmap(troncos3, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case TRONCOS5:
			al_draw_bitmap(troncos5, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case TORTUGAS2:
			al_draw_bitmap(tortugas2, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case TORTUGAS3:
			al_draw_bitmap(tortugas3, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		case AUTOBUS:
			al_draw_bitmap(autobus, objetos[i].x * SIZEOFSKIN, objetos[i].fila * SIZEOFSKIN, 0);
			break;

		default:
			break;
		}
	}
	return;
}

static void imprimirVidas(void)	// Imprime las vidas (corazones) en la posición correspondiente
{
	int i;
	for( i = 0 ; i < player.vidas ; i++)
	{
		al_draw_bitmap(vidas, i * SIZEOFSKIN, 15 * SIZEOFSKIN, 0);
	}
	return;
}

static void imprimirMetas(void)	// Imprime las 5 metas
{
	int i;
	for( i = 0 ; i < METAS ; i++)
	{
		if(metas[i].estado == ON)
		{
			al_draw_bitmap_region(skin[skinElegida-10], 0, 2*SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN, metas[i].x * SIZEOFSKIN, metas[i].fila * SIZEOFSKIN, 0);
		}
	}
	return;
}

static void imprimirPlayer(int x, int y, int facing)	// Imprime el jugador, en cada situacion posible (calle, tronco, o tortugar)
{
	switch (player.estado)
	{
	case VIVA:
	case ENCIMATRO:
	case ENCIMATOR:
		al_draw_bitmap_region(skin[skinElegida-10], 0, SIZEOFSKIN * facing, SIZEOFSKIN, SIZEOFSKIN, SIZEOFSKIN * x, SIZEOFSKIN * y, 0);	// Imprime la skin seleccionada
		break;
	}
	return;
}

int init(void)	// inicializa todo lo vinculado con allegro
{
	if(!al_init())	// Inicializa allegro
	{
		printf("Error al inicilizar allegro");
		return ERROR;
	}
	
	ventana = al_create_display(WIDTH, HEIGTH);	// La crea
	
	if(!ventana)
	{
		printf("Error al crear una ventana"); // si hay error
		return ERROR;
	}
	
	al_set_window_title(ventana, "Frogger");	// Nombre
	al_set_window_position(ventana, 650, 200);	// Posición, pusimos una cualquiera
	
	if(!al_init_font_addon())	// Los módulos a continuación verifican que no hayan errores ya sea en las fuentes, mouse, teclado, etc
	{
		printf("Error al inicilizar modulo de fuentes");
		return ERROR;
	}
	
	if(!al_init_ttf_addon())
	{
		printf("Error al inicilizar modulo de fuentes");
		return ERROR;
	}
	
	if(!al_init_primitives_addon())
	{
		printf("Error al inicilizar modulo de primitives");
		return ERROR;
	}
	
	if(!al_install_mouse())
	{
		printf("Error al inicilizar modulo del mouse");
		return ERROR;
	}
	
	if(!al_install_keyboard())
	{
		printf("Error al inicilizar modulo del teclado");
		return ERROR;
	}
	
	if(!al_init_image_addon())
	{
		printf("Error al inicilizar modulo imagenes");
		return ERROR;
	}
	
	colaEventos = al_create_event_queue(); // crea la cola de eventos
	
	if(!colaEventos)
	{
		printf("Error al crear la cola de eventos");
		return ERROR;
	}
	
	al_register_event_source(colaEventos, al_get_mouse_event_source()); // se agrega el mouse y teclado a la cola de eventos
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	
	if(path()) //funcion que se encarga de vincular todos los archivos resources
	{
		return ERROR;
	}
	
	transparente = al_map_rgb(255, 0, 255);		// Crea los colores 
	blanco = al_map_rgb(255, 255, 255);
	azul = al_map_rgb(24, 30, 120);
	verde = al_map_rgb(0, 255, 0);
	verdeoscuro = al_map_rgb(0, 75, 0);
	negro = al_map_rgb(0, 0, 0);
	
	al_convert_mask_to_alpha(logo, transparente);// le quita el fondo a las imagenes
	al_convert_mask_to_alpha(pasto, transparente); 
	al_convert_mask_to_alpha(pastoMenu, transparente);
	al_convert_mask_to_alpha(pastoGame, transparente);
	al_convert_mask_to_alpha(vidas, transparente);
	al_convert_mask_to_alpha(muertes, transparente);
	al_convert_mask_to_alpha(auto1, transparente);
	al_convert_mask_to_alpha(auto2, transparente);
	al_convert_mask_to_alpha(auto3, transparente);
	al_convert_mask_to_alpha(auto4, transparente);
	al_convert_mask_to_alpha(autobus, transparente);
	al_convert_mask_to_alpha(troncos3, transparente);
	al_convert_mask_to_alpha(troncos5, transparente);
	al_convert_mask_to_alpha(tortugas2, transparente);
	al_convert_mask_to_alpha(tortugas3, transparente);
	
	al_convert_mask_to_alpha(skin[RANA-10], transparente);
	al_convert_mask_to_alpha(skin[GATO-10], transparente);
	al_convert_mask_to_alpha(skin[PATO-10], transparente);
	al_convert_mask_to_alpha(skin[PERRO-10], transparente);
	al_convert_mask_to_alpha(skin[CHANCHO-10], transparente);
	al_convert_mask_to_alpha(skin[AVESTRUZ-10], transparente);
	
	al_set_display_icon(ventana, logo); // setea el logo a la ventana
	
	return 0;
}

static int path(void)	// vincula los archivos resources con sus respectivas variables
{
	int i;

	logo = al_load_bitmap("imagenes/icon.bmp");
	
	if(!logo)	// Verifica que no hayan errores en la creación de los bitmaps 
	{
		printf("Error al cargar el bitmap logo");
		return ERROR;
	}
	
	pasto = al_load_bitmap("imagenes/pasto.bmp");
	
	if(!pasto)
	{
		printf("Error al cargar el bitmap pasto");
		return ERROR;
	}
	
	pastoMenu = al_load_bitmap("imagenes/pastoMenu.bmp");
	
	if(!pastoMenu)
	{
		printf("Error al cargar el bitmap pastoMenu");
		return ERROR;
	}
	
	pastoGame = al_load_bitmap("imagenes/pastoGame.bmp");
	
	if(!pastoGame)
	{
		printf("Error al cargar el bitmap pastoGame");
		return ERROR;
	}
	
	agua = al_load_bitmap("imagenes/agua.bmp");
	
	if(!agua)
	{
		printf("Error al cargar el bitmap agua");
		return ERROR;
	}
	
	aguaMenu = al_load_bitmap("imagenes/aguaMenu.bmp");
	
	if(!aguaMenu)
	{
		printf("Error al cargar el bitmap aguaMenu");
		return ERROR;
	}
	
	calle = al_load_bitmap("imagenes/calle.bmp");
	
	if(!pasto)
	{
		printf("Error al cargar el bitmap calle");
		return ERROR;
	}
	
	nada = al_load_bitmap("imagenes/nada.bmp");
	
	if(!nada)
	{
		printf("Error al cargar el bitmap nada");
		return ERROR;
	}
	
	vidas = al_load_bitmap("imagenes/heart.bmp");
	
	if(!vidas)
	{
		printf("Error al cargar el bitmap vidas");
		return ERROR;
	}
	
	muertes = al_load_bitmap("imagenes/muertes.bmp");
	
	if(!muertes)
	{
		printf("Error al cargar el bitmap muertes");
		return ERROR;
	}
	
	auto1 = al_load_bitmap("imagenes/auto1.bmp");
	
	if(!auto1)
	{
		printf("Error al cargar el bitmap auto1");
		return ERROR;
	}
	
	auto2 = al_load_bitmap("imagenes/auto2.bmp");
	
	if(!auto2)
	{
		printf("Error al cargar el bitmap auto2");
		return ERROR;
	}
	
	auto3 = al_load_bitmap("imagenes/auto3.bmp");
	
	if(!auto3)
	{
		printf("Error al cargar el bitmap auto3");
		return ERROR;
	}
	
	auto4 = al_load_bitmap("imagenes/auto4.bmp");
	
	if(!auto4)
	{
		printf("Error al cargar el bitmap auto4");
		return ERROR;
	}
	
	autobus = al_load_bitmap("imagenes/autobus.bmp");
	
	if(!autobus)
	{
		printf("Error al cargar el bitmap autobus");
		return ERROR;
	}
	
	troncos3 = al_load_bitmap("imagenes/troncos3.bmp");
	
	if(!troncos3)
	{
		printf("Error al cargar el bitmap troncos3");
		return ERROR;
	}
	
	troncos5 = al_load_bitmap("imagenes/troncos5.bmp");
	
	if(!troncos5)
	{
		printf("Error al cargar el bitmap troncos5");
		return ERROR;
	}
	
	tortugas2 = al_load_bitmap("imagenes/tortugas2.bmp");
	
	if(!tortugas2)
	{
		printf("Error al cargar el bitmap tortugas2");
		return ERROR;
	}
	
	tortugas3 = al_load_bitmap("imagenes/tortugas3.bmp");
	
	if(!tortugas3)
	{
		printf("Error al cargar el bitmap tortugas3");
		return ERROR;
	}
	
	
	//distintos tamanos de fuente
	gameria[0] = al_load_font("fuente/GAMERIA.ttf", 100, 0);
	gameria[1] = al_load_font("fuente/GAMERIA.ttf", 30, 0);
	gameria[2] = al_load_font("fuente/GAMERIA.ttf", 35, 0);
	gameria[3] = al_load_font("fuente/GAMERIA.ttf", 70, 0);
	gameria[4] = al_load_font("fuente/GAMERIA.ttf", 55, 0);
	gameria[5] = al_load_font("fuente/GAMERIA.ttf", 40, 0);
	gameria[6] = al_load_font("fuente/GAMERIA.ttf", 45, 0);

	for(i = 0 ; i <= 6 ; i++)
	{
		if(!gameria[i])
		{
			printf("Error al cargar las fuentes");
			return ERROR;
		}
	}
	
	//distintas skins
	skin[RANA-10] = al_load_bitmap("imagenes/rana.bmp");
	skin[GATO-10] = al_load_bitmap("imagenes/gato.bmp");
	skin[PATO-10] = al_load_bitmap("imagenes/pato.bmp");
	skin[PERRO-10] = al_load_bitmap("imagenes/perro.bmp");
	skin[CHANCHO-10] = al_load_bitmap("imagenes/chancho.bmp");
	skin[AVESTRUZ-10] = al_load_bitmap("imagenes/avestruz.bmp");
	
	for(i = 0 ; i <= 5 ; i++)
	{
		if(!skin[i])
		{
			printf("Error al cargar las skins");
			return ERROR;
		}
	}
	
	return 0;
}

void closeit(void)  // Cierra todas las actividades, variables, bitmaps, cola de eventos...
{
	int i;
    al_destroy_event_queue(colaEventos);
    al_destroy_display(ventana);
    for (i = 0; i <= 6; i++)
    {
        al_destroy_font(gameria[i]);
    }
     for (i = 0; i <= 5; i++)
    {
        al_destroy_bitmap(skin[i]);
    }
    al_destroy_bitmap(icon);
    al_destroy_bitmap(pasto);
    al_destroy_bitmap(pastoMenu);
    al_destroy_bitmap(pastoGame);
    al_destroy_bitmap(agua);
    al_destroy_bitmap(aguaMenu);
    al_destroy_bitmap(calle);
    al_destroy_bitmap(nada);
    al_destroy_bitmap(vidas);
    al_destroy_bitmap(muertes);
    al_destroy_bitmap(auto1);
    al_destroy_bitmap(auto2);
    al_destroy_bitmap(auto3);
    al_destroy_bitmap(auto4);
    al_destroy_bitmap(troncos3);
    al_destroy_bitmap(troncos5);
    al_destroy_bitmap(tortugas2);
    al_destroy_bitmap(tortugas3);
    al_destroy_bitmap(autobus);
    return;
}

