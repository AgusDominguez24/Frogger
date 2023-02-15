#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "backend.h"
#include "constantes.h"
#include "estructuras.h"

#ifdef ALLEGRO

#include "frontendAl.h" // libreria de frontend si se usa allegro

#endif //ALLEGRO

#ifdef RASPI

void* playerRa(); //utilizamos un thread adicional cuando estamos en raspi
#include "frontendRa.h" // libreria de frontend si se usa raspi

#endif //RASPI

/* VARIABLES GLOBALES */
unsigned int nivel = NIVEL0; // nivel actual del jugador
unsigned int pagina = MENUP; // pagina actual en la que se encuentra el jugador
unsigned int arregloFondo[16] = {0}; // arreglo que se completa con las diferentes filas. Ver en la funcion completarFondo
unsigned int high_score = 0; // varible donde se guarda la mayor puntuacion obtenida( no se borra con la compilacion )
unsigned int timer = TIEMPO; // tiempo actual de juego

//Variable tipo player_t para poder trabajar con el jugador
player_t player = {POSINIX, POSINIY, POSINIY, VIVA, UP, MAXVIDAS, NONE, ON}; // struct player que posee toda la info del jugador

//Variable de tipo metas_t con las posiciones de las metas
metas_t metas[5] = {{1, 2, OFF, META}, // arreglo de struct donde posee toda la info de las distintas metas
					{4, 2, OFF, META},
					{7, 2, OFF, META},
					{10, 2, OFF, META},
					{13, 2, OFF, META}};

//Variables tipo objetos_t 
objetos_t objetos[OBJETOS] = { { 0, CALLE5, ON, AUTO1, DER, BAJA}, // arreglo de struct, toda la info de los objetos del juego
						{ 4, CALLE5, ON, AUTO1, DER, BAJA},
						{ 8, CALLE5, ON, AUTO1, DER, BAJA},
						{ 12, CALLE5, ON, AUTO1, DER, BAJA},
						{ 2, CALLE4, ON, AUTO4, IZQ, ALTA},
						{ 6, CALLE4, ON, AUTO4, IZQ, ALTA},
						{ 10, CALLE4, ON, AUTO4, IZQ, ALTA},
						{ 0, CALLE3, ON, AUTO2, DER, MEDIA},
						{ 7, CALLE3, ON, AUTO2, DER, MEDIA},
						{ 11, CALLE2, ON, AUTO3, IZQ, ALTA},
						{ 3, CALLE1, ON, AUTOBUS, IZQ, BAJA},
						{ 10, CALLE1, ON, AUTOBUS, IZQ, BAJA},
						{ 2, AGUA1, ON, TORTUGAS3, IZQ, MEDIA},
						{ 7, AGUA1, ON, TORTUGAS3, IZQ, MEDIA},
						{ 12, AGUA1, ON, TORTUGAS3, IZQ, MEDIA},
						{ 0, AGUA3, ON, TORTUGAS2, IZQ, BAJA},
						{ 4, AGUA3, ON, TORTUGAS2, IZQ, BAJA},
						{ 8, AGUA3, ON, TORTUGAS2, IZQ, BAJA},
						{ 12, AGUA3, ON, TORTUGAS2, IZQ, BAJA},
						{ TRONCO3INI, AGUA2, ON, TRONCOS3, DER, MEDIA},
						{ 4, AGUA2, ON, TRONCOS3, DER, MEDIA},
						{ 11, AGUA2, ON, TRONCOS3, DER, MEDIA},
						{ 7, AGUA4, ON, TRONCOS5, DER, MEDIA},
						{ TRONCO5INI, AGUA4, ON, TRONCOS5, DER, MEDIA},
						{ 0, AGUA5, ON, TORTUGAS3, IZQ, BAJA},
						{ 9, AGUA5, ON, TORTUGAS3, IZQ, BAJA}};


/* VARIABLES LOCALES */
static unsigned int plataforma = PLATAFORMA; // al ser dificil distinguir bien el juego en la raspi, consideramos
//utilizar una variable que dependiendo de la plataforma, el juego va mas rapido o mas lento
static unsigned int salir = FALSE; // variable para terminar el juego
static unsigned int pausa = FALSE; // variable para pausar el juego, si es TRUE no avanza el tiempo ni nada del juego


/* FUNCIONES */
int main(void)
{

	if(init()) // funcion para inicializar todo lo necesario, tanto para allegro o para raspi
	{
		salir = TRUE;
	}
	
	compareScore(); // obtenemos el highscore del archivo highscore.txt
	
	//Crea thread solo para RasPi
	#ifdef RASPI
	pthread_t thread_player_ra;
	pthread_create(&thread_player_ra, NULL, playerRa, NULL);
	#endif

	//Crea threads para que distintos objetos puedan tener distintas velocidades
	pthread_t thread_velocidad1;
	pthread_t thread_velocidad2;
	pthread_t thread_velocidad3;
	pthread_t thread_logic;
	pthread_t thread_tiempo;
	pthread_create(&thread_velocidad1, NULL, velocidad1, NULL);
	pthread_create(&thread_velocidad2, NULL, velocidad2, NULL);
	pthread_create(&thread_velocidad3, NULL, velocidad3, NULL);
	pthread_create(&thread_logic, NULL, logic, NULL);
	pthread_create(&thread_tiempo, NULL, tiempo, NULL);
	
	
	
	while(!salir)
	{
		renderizado(); // se encarga de todo lo que es imprimir, tanto en allegro como en raspi
		
		switch(pagina)
		{
		case NEWGAME:
		
			player.estado = isTheFrogDead(player.x, player.y); //Devuelve el estado de la rana
			
			
			//Si se choca o cae al agua tiene 1 vida menos
			if (player.estado == AHOGADA || player.estado == CHOCADA)
			{
				player.vidas--; // cada vez que la rana muere, vidas--
				
				#ifdef ALLEGRO
				pagina = MUERTE; // en allegro hay una pagina muerte, para mostrar la animacion de muerte
				#endif
				#ifdef RASPI
				player.x = POSINIX;
				player.y = POSINIY;
				#endif
			}

			
			if (player.vidas == 0) //Si se queda sin vidas:
			{
				pagina = GAMEOVER;
				compareScore(); //Compara con el previo puntaje mayor
				player.vidas = MAXVIDAS; //se resetean las vidas
				pausa = TRUE; // pausa es verdadero
				timer = TIEMPO; //se resetea el tiempo
				player.x = POSINIX; //Resetea posicion inicial del jugador
				player.y = POSINIY;
				player.y2 = POSINIY;
				nivel = NIVEL0; // se resetea el tiempo
				//Resetea las metas
				for (int i = 0; i < METAS; i++)
				{
					metas[i].estado = OFF;
				}
			}
			
			if (isNextLevel()) //hay que pasar de nivel?
			{
				nextLevel(); //se pasa de nivel
			}
			break;
			
		}
		
	}
	
	//pthread_join de todas los threads que ya se terminaron de usar
	
	pthread_join(thread_velocidad1, NULL);
	pthread_join(thread_velocidad2, NULL);
	pthread_join(thread_velocidad3, NULL);
	pthread_join(thread_logic, NULL);
	pthread_join(thread_tiempo, NULL);

	printf("ME FUI\n"); //ME FUI

	#ifdef RASPI
	pthread_join(thread_player_ra, NULL);
	#endif

	//cierra todas las actividades, variables, colas de eventos, ... de Allegro o de la raspi
	closeit();

	return 0;
}

//Funcion que prepara el fondo del juego dependiendo de en qué parte del juego te encuentres
static void completarFondo(unsigned int arregloFondo[16])
{
	if(pagina == NEWGAME || pagina == pausa || pagina == MUERTE)
	{
		arregloFondo[0] = PASTOGAME;
		arregloFondo[1] = FALSE;
		arregloFondo[2] = AGUA;
		arregloFondo[3] = FALSE;
		arregloFondo[4] = FALSE;
		arregloFondo[5] = FALSE;
		arregloFondo[6] = FALSE;
		arregloFondo[7] = FALSE;
		arregloFondo[8] = PASTO;
		arregloFondo[9] = CALLE;
		arregloFondo[10] = CALLE;
		arregloFondo[11] = CALLE;
		arregloFondo[12] = CALLE;
		arregloFondo[13] = CALLE;
		arregloFondo[14] = PASTO;
		arregloFondo[15] = NADA;
	}
	else
	{
		int i;
		arregloFondo[0] = PASTOMENU;
		arregloFondo[1] = AGUAMENU;
		for( i = 2 ; i < MAXCOL ; i++)
		{
			arregloFondo[i] = FALSE;
		}
	}
	return;
}

//Funcion que actualiza el estado del jugador
static unsigned int isTheFrogDead(int x, int y)
{
	unsigned int estado = VIVA;

	//Para todos los objetos
	for (int i = 0; i < OBJETOS; i++)
	{
		//Si se encuentra en la calle
		if (y >= CALLE1 && y <= CALLE5)
		{
			//Dependiendo de si es Auto o Autobus
			switch (objetos[i].tipo)
			{
				case AUTO1:
				case AUTO2:
				case AUTO3:
				case AUTO4:
					if (x == objetos[i].x && y == objetos[i].fila)
					{//Misma posicion de x e y?
						estado = CHOCADA;
					}
					break;
				case AUTOBUS:
					if ((x == objetos[i].x || x == objetos[i].x + 1) && y == objetos[i].fila)
					{//Misma posicion de x e y o de (x+1) e y? (el autobus ocupa 2 lugares)
						estado = CHOCADA;
					}
					break;

				default:
					break;
			}
		}
		//Si se encuentra en el agua
		else if (y >= AGUA1 && y <= AGUA5)
		{
			//Depende del tipo de objeto
			switch (objetos[i].tipo)
			{
				case TRONCOS3:
					if ((y == objetos[i].fila) && (x == objetos[i].x || x == objetos[i].x + 1 || x == objetos[i].x + 2) )
					{//Revisa si la posicion del jugador es identica a alguna de las posiciones del objeto
						estado = ENCIMATRO;
					}
					break;
				case TRONCOS5:
					if ((y == objetos[i].fila) && (x == objetos[i].x || x == objetos[i].x + 1 || x == objetos[i].x + 2 || x == objetos[i].x + 3 || x == objetos[i].x + 4))
					{//Revisa si la posicion del jugador es identica a alguna de las posiciones del objeto
						estado = ENCIMATRO;
					}
					break;
				case TORTUGAS2:
					if ((y == objetos[i].fila) && (x == objetos[i].x || x == objetos[i].x + 1 ))
					{//Revisa si la posicion del jugador es identica a alguna de las posiciones del objeto
						estado = ENCIMATOR;
					}
					break;
				case TORTUGAS3:
					if ((y == objetos[i].fila) && (x == objetos[i].x || x == objetos[i].x + 1 || x == objetos[i].x + 2) )
					{//Revisa si la posicion del jugador es identica a alguna de las posiciones del objeto
						estado = ENCIMATOR;
					}
					break;
				
				default:
					estado = AHOGADA;//Si no está encima de un objeto está en el agua
					break;
			}
		}
	}
	return estado;//Devuelve el estado del jugador
}

//Comprueba si el jugador llegó a todas las metas
static unsigned int isNextLevel(void)
{
	unsigned int nextLevel = TRUE;
	int i;
	for ( i = 0 ; i < METAS ; i++)
	{
		nextLevel *= metas[i].estado;
	}
	return nextLevel;
}

//Configura el nuevo nivel
static void nextLevel(void)
{
	int i;
	//Resetea las metas para el nuevo nivel
	for (i = 0; i < METAS; i++)
	{
		metas[i].estado = OFF;
	}
	
	player.puntaje += 5; //Al completar el nivel se le suman 5 puntos
	
	//Pasas de nivel
	if (nivel > 1)
	{
		nivel--;
	}
	//Completaste el ultimo nivel?
	else
	{
		pagina = GAMEOVER;
		pausa = TRUE;
	}

	return;
}

//Funcion que compara player.puntaje con el high score
static void compareScore(void)
{
   FILE* fp = fopen("high_score.txt", "r");//Crea .txt para guardar el mayor puntaje obtenido
   if (fp == NULL) 
   {//Si está vacio
      high_score = 0;
   }
   else 
   {//Si no
      fscanf(fp, "%d", &high_score); //Imprime el mayor puntaje guardado
      fclose(fp);
   }

   // compara player.puntaje con el high score
   if (player.puntaje > high_score) 
   {//Nuevo puntaje mayor al highscore anterior?

      // update high score

      high_score = player.puntaje;

      // guardamos score como highscore al archivo txt

      FILE* fp = fopen("high_score.txt", "w");

      if (fp == NULL) 
      {
         printf("Error abriendo el archivo\n");
      }
      else 
      {
         fprintf(fp, "%d", high_score);
         fclose(fp);
      }
   }
   return;
}


/* FUNCIONES SOLO DE ALLEGRO */

#ifdef ALLEGRO

//Funcion para realizar acciones dependiendo del boton apretado
static void accion(int botonAl)
{
	switch(pagina)
	{
	case MENUP:

		//Opciones del Menú principal
		if (botonAl == NEWGAME)
		{
			pagina = NEWGAME;
		}
		else if (botonAl == SKINS)
		{
			pagina = SKINS;
		}
		else if (botonAl == EXIT)
		{
		salir = TRUE;
		}
		break;
		
	case SKINS:

		//Opciones del Menú skins
		if (botonAl == RANA)
		{
			skinElegida = RANA;
			pagina = MENUP;
		}
		else if (botonAl == GATO)
		{
			skinElegida = GATO;
			pagina = MENUP;
		}
		else if (botonAl == PATO)
		{
			skinElegida = PATO;
			pagina = MENUP;
		}
		else if (botonAl == PERRO)
		{
			skinElegida = PERRO;
			pagina = MENUP;
		}
		else if (botonAl == CHANCHO)
		{
			skinElegida = CHANCHO;
			pagina = MENUP;
		}
		else if (botonAl == AVESTRUZ)
		{
			skinElegida = AVESTRUZ;
			pagina = MENUP;
		}
		else if (botonAl == BACK)
		{
			pagina = MENUP;
		}
		break;
		
	case PAUSA:

		//Opciones del pausa
		if (botonAl == REANUDAR)
		{
			pagina = NEWGAME;
			pausa = FALSE;
		}
		else if(botonAl == ENDGAME)
		{
			pagina = MENUP;
			player.vidas = MAXVIDAS;
			player.x = POSINIX;
			player.y = POSINIY;
			nivel = NIVEL0;
			for (int i = 0; i < METAS; i++)
			{
				metas[i].estado = OFF;
			}
			completarFondo(arregloFondo);
			player.puntaje = 0;
			player.y2 = POSINIY;
			timer = TIEMPO;
			pausa = FALSE;
		}
	case GAMEOVER:

		//Opciones del menú gameover
		if (botonAl == QUIT)
		{
			pagina = MENUP;
			player.vidas = MAXVIDAS;
			player.x = POSINIX;
			player.y = POSINIY;
			nivel = NIVEL0;
			for (int i = 0; i < METAS; i++)
			{
				metas[i].estado = OFF;
			}
			completarFondo(arregloFondo);
			player.puntaje = 0;
			player.y2 = POSINIY;
			timer = TIEMPO;
			pausa = FALSE;
		}
		break;
	}
	return;
}

//Funcion para reconocimiento del mouse sobre una opcion del menú en el que se encuentre
static int sobreBoton(int x, int y)
{
	int sobreelboton = FALSE;
	switch (pagina)
	{//Se fija que las coordenadas coincidan con las de un botón
	case MENUP:
		if (x > fontleftside(gameria[5], "NEW GAME") && x < fontrightside(gameria[5], "NEW GAME") && y > 300 && y < 300 + al_get_font_line_height(gameria[5]))
		{
			sobreelboton = NEWGAME;
		}
		else if (x > fontleftside(gameria[5], "SKINS") && x < fontrightside(gameria[5], "SKINS") && y > 370 && y < 370 + al_get_font_line_height(gameria[5]))
		{
			sobreelboton = SKINS;
		}
		else if (x > fontleftside(gameria[5], "EXIT") && x < fontrightside(gameria[5], "EXIT") && y > 440 && y < 440 + al_get_font_line_height(gameria[5]))
		{
			sobreelboton = EXIT;
		}
		else
		{
			sobreelboton = FALSE;
		}
		break;
	
	case NEWGAME:

		break;

	case SKINS:

		if (x > fontleftside(gameria[1], "BACK") && x < fontrightside(gameria[1], "BACK") && y > 500 && y < 500 + al_get_font_line_height(gameria[1]))
		{
			sobreelboton = BACK;
		}
		else if (x > 105 && x < 155 && y > 305 && y < 355)
		{
			sobreelboton = RANA;
		}
		else if (x > 165 && x < 215 && y > 305 && y < 355)
		{
			sobreelboton = GATO;
		}
		else if (x > 225 && x < 275 && y > 305 && y < 355)
		{
			sobreelboton = PATO;
		}
		else if (x > 285 && x < 335 && y > 305 && y < 355)
		{
			sobreelboton = PERRO;
		}
		else if (x > 345 && x < 395 && y > 305 && y < 355)
		{
			sobreelboton = CHANCHO;
		}
		else if (x > 405 && x < 455 && y > 305 && y < 355)
		{
			sobreelboton = AVESTRUZ;
		}
		else
		{
			sobreelboton = FALSE;
		}
		break;

	case PAUSA:
		if (x > fontleftside(gameria[1], "REANUDAR") && x < fontrightside(gameria[1], "REANUDAR") && y > 250 && y < 250 + al_get_font_line_height(gameria[1]))
		{
			sobreelboton = REANUDAR;
		}
		else if (x > fontleftside(gameria[1], "ENDGAME") && x < fontrightside(gameria[1], "ENDGAME") && y > 300 && y < 300 + al_get_font_line_height(gameria[1]))
		{
			sobreelboton = ENDGAME;
		}
		else
		{
			sobreelboton = FALSE;
		}
		break;

	case GAMEOVER:
		if (x > fontleftside(gameria[1], "QUIT") && x < fontrightside(gameria[1], "QUIT") && y > 400 && y < 400 + al_get_font_line_height(gameria[1]))
		{
			sobreelboton = QUIT;
		}
		else
		{
			sobreelboton = FALSE;
		}
		break;
	}
	return sobreelboton;
}
#endif


/* FUNCIONES DE THREAD */

#ifdef RASPI

void* playerRa() // para que el jugador y el tiempo titilen, asi se distinguen mejor
{
	while (!salir)
	{
		if (pagina == NEWGAME)
		{
			if (player.mostrar == ON)
			{
				player.mostrar = OFF;
			}
			else
			{
				player.mostrar = ON;
			}

			if (mostrarTimer == ON)
			{
				mostrarTimer = OFF;
			}
			else
			{
				mostrarTimer = ON;
			}
		}
		usleep(100000);
	}
	pthread_exit(NULL);
}
#endif

//Funcion para el funcionamiento del timer
void* tiempo()
{
	while(!salir)
	{
		if(pagina == NEWGAME)
		{
			usleep(1000000);
			if(timer > 0)
			{//Disminuye el tiempo restante
				timer--;
				
				if(pagina!=GAMEOVER)
				{//Cada segundo suma un punto
					player.puntaje+=1;
				}
			}
			else
			{
				timer = TIEMPO;//Resetea el timer
				pagina = GAMEOVER;//Se te acabó el tiempo? Perdiste
				pausa = TRUE;
			}
		}
		
		
	}
	pthread_exit(NULL);
}

//Función con la logica del funcionamiento juego
void* logic()
{
	while(!salir)
	{
		completarFondo(arregloFondo);//Configura el fondo

		#ifdef ALLEGRO //Permite que se recorran todos los botones con las flechas del teclado
		al_wait_for_event(colaEventos, &evento);
		if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(pagina)
			{//Dependiendo de en que parte del juego estés
			case MENUP: 
				if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					if (botonAl < EXIT)
					{
						botonAl++;
					}
					else
					{
						botonAl = EXIT;
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					
					if(botonAl > EXIT)
					{
						botonAl = EXIT;
					}
					else if (botonAl > NEWGAME)
					{
						botonAl--;
					}
					else
					{
						botonAl = NEWGAME;
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					accion(botonAl);// manda el boton y realiza la respectiva accion
				}
				break;

			case NEWGAME:
				//Movimiento del jugador usando el teclado
				if (evento.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					if (player.facing != UP && pausa == FALSE)
					{//Jugador apunta hacia arriba al presionar flecha hacia arriba cuando apunta a otro lado
						player.facing = UP;
					}
					else
					{//De estar apuntando hacia arriba
						if (player.y > 3)
						{//Mueve al jugador hacia arriba
							player.y--;
							if (player.y < player.y2)
							{//Si no había pasado antes por esa fila
								player.puntaje += 1;//Suma un punto
								player.y2 = player.y;
							}
						}
						else if (player.y == 3)
						{//En la fila de las metas
							for (int i = 0; i < METAS; i++)
							{//Revisa meta por meta

								if (player.x == metas[i].x)
								{
									if (metas[i].estado == OFF)
									{
										metas[i].estado = ON;//Reconoce que ahora la meta está ocupada
										player.puntaje += 2;//Suma 2 puntos
										timer += 10;//Aumenta el tiempo 10 segundos cada vez que se llega a una meta
										player.x = POSINIX;//Regresa al jugador a la posicion inicial
										player.y = POSINIY;
										player.y2 = POSINIY;
										break;
									}
									else
									{
										player.y = 3;//¿WHAT?
										break;
									}
								}
							}
						}
						else
						{
							player.y = 3;//¿WHAT_2.0?
						}
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					if (player.facing != DOWN && pausa == FALSE)
					{//Idem a lo anterior pero para ir hacia abajo
						player.facing = DOWN;
					}
					else
					{
						if (player.y < 14)
						{
							player.y++;
						}
						else
						{
							player.y = 14;
						}
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
				{//Idem pero hacia la izquierda
					if (player.facing != LEFT && pausa == FALSE)
					{
						player.facing = LEFT;
					}
					else
					{
						if (player.x > 0)
						{
							player.x--;
						}
						else
						{
							player.x = 0;
						}
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				{//Idem pero hacia la derecha
					if (player.facing != RIGHT && pausa == FALSE)
					{
						player.facing = RIGHT;
					}
					else
					{
						if (player.x < 15)
						{
							player.x++;
						}
						else
						{
							player.x = 15;
						}
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{//Al presionar ESC se va al menú de pausa
					pagina = PAUSA;
					botonAl = FALSE;
					pausa = TRUE;
				}

				break;

			case SKINS: 
				if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{//Si se presiona ESC vuelve al menu principal
					pagina = MENUP; 
				}
				if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					botonAl = BACK;
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					if(!(botonAl >= RANA && botonAl <= AVESTRUZ))
					{
						botonAl = PERRO;
					}
				}
				else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
				{
					if (botonAl > RANA && botonAl <= AVESTRUZ)
					{
						botonAl--;
					}
					else
					{
						botonAl = RANA;
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				{
					if (botonAl < AVESTRUZ && botonAl >= RANA)
					{
						botonAl++;
					}
					else
					{
						botonAl = AVESTRUZ;
					}
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					accion(botonAl);//Manda el boton y realiza la respectiva accion
				}
				break;

			case PAUSA:
				if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					pagina = NEWGAME;
					pausa = FALSE;
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					accion(botonAl); //Recibe el boton y realiza la respectiva accion
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					botonAl = REANUDAR;
				}
				else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					botonAl = ENDGAME;
				}
				break;
			
			case GAMEOVER:
		
			if (evento.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				botonAl = QUIT;
			}
			else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				botonAl = QUIT;
			}
			else if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				accion(botonAl);
			}
		
			break;
			
			}
		}
		else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{//Para el uso del mouse
			mouse.x = evento.mouse.x;
			mouse.y = evento.mouse.y;
			botonAl = sobreBoton(mouse.x, mouse.y);
		}
		else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			accion(botonAl);
		}
		#endif
		
		#ifdef RASPI
		joystick = joy_read();
		//Para el movimiento del Joystick en la RasPi
		switch (pagina)
		{
		case MENUP:
			if (joystick.x < -10 && joystick.y > 10)
			{
				botonRa = BOTON1;
			}
			else if (joystick.x > 10 && joystick.y > 10)
			{
				botonRa = BOTON2;
			}
			else if (joystick.x > 10 && joystick.y < -10)
			{
				botonRa = BOTON3;
			}
			else if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}
			if (joystick.sw == J_PRESS)
			{//Cuando se presiona un botón:
				switch (botonRa)
				{//Realiza acciones dependiendo de que opción se elige

				case BOTON2:
					pagina = NEWGAME;
					botonRa = BOTONOFF;
					break;

				case BOTON3:
					pagina = HIGHSCORE;
					botonRa = BOTONOFF;
					break;

				case BOTON4:
					disp_clear();
					disp_update();
					salir = TRUE;
					botonRa = BOTONOFF;
					break;

				}

			}
			break;

		case NEWGAME:

			//Dentro de la partida para el movimiento del jugador
			if (joystick.x < - 10)
			{
				if (player.x > 0)
				{
					player.x--;
				}
				else
				{
					player.x = 0;
				}
			}
			else if (joystick.x > 10)
			{
				if (player.x < 15)
				{
					player.x++;
				}
				else
				{
					player.x = 15;
				}
			}
			else if (joystick.y < - 10)
			{
				if (player.y < 14)
				{
					player.y++;
				}
				else
				{
					player.y = 14;
				}
			}
			else if (joystick.y > 10)
			{
				if (player.y > 3)
				{
					player.y--;
					if (player.y < player.y2)
					{
						player.puntaje += 1;
						player.y2 = player.y;
					}
				}
				else if (player.y == 3)
				{//En la fila de las metas
					for (int i = 0; i < METAS; i++)
					{//Revisa meta por meta

						if (player.x == metas[i].x)
						{
							if (metas[i].estado == OFF)
							{
								metas[i].estado = ON;//Reconoce que ahora la meta está ocupada
								player.puntaje += 2;//Suma 2 puntos
								timer += 10;//Aumenta el tiempo 10 segundos cada vez que se lleg a una meta
								player.x = POSINIX;//Regresa al jugador a la posicion inicial
								player.y = POSINIY;
								player.y2 = POSINIY;
								break;
							}
							else
							{
								player.y = 3;//¿IDK?
								break;
							}
						}
					}
				}
				else
				{
					player.y = 3;//¿IDK_2.0?
				}
			}
			if (joystick.sw == J_PRESS)
			{//Al presionar el joystick se va al menú de pausa
				pagina = PAUSA;
				botonRa = BOTONOFF;
				pausa = TRUE;
			}


			break;

		case HIGHSCORE: //ESTAS 2 NO LAS ENTIENDO

			if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}

			if (joystick.sw == J_PRESS)
			{
				switch (botonRa)
				{

				case BOTON4:
					pagina = MENUP;
					botonRa = BOTONOFF;
					break;

				}

			}

			break;

		case SCORE1:

			if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}

			if (joystick.sw == J_PRESS)
			{
				switch (botonRa)
				{

				case BOTON4:
					pagina = PAUSA;
					botonRa = BOTONOFF;
					break;

				}

			}
			

			break;

		case PAUSA:
			if (joystick.x > 10 && joystick.y > 10)
			{
				botonRa = BOTON2;
			}
			else if (joystick.x > 10 && joystick.y < -10)
			{
				botonRa = BOTON3;
			}
			else if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}
			
			if (joystick.sw == J_PRESS)
			{
				switch (botonRa)
				{//Opciones del menú de pausa

				case BOTON2:
				//Volver al juego
					pagina = NEWGAME;
					botonRa = BOTONOFF;
					pausa = FALSE;
					break;

				case BOTON3:
				//Mostrar puntaje actual
					pagina = SCORE1;
					botonRa = BOTONOFF;
					break;

				case BOTON4:
				//Volver al menú principal
					pagina = MENUP;
					player.vidas = MAXVIDAS;//Resetea vidas del jugador
					player.x = POSINIX;//Resetea posicion inicial del jugador
					player.y = POSINIY;
					player.y2 = POSINIY;
					nivel = NIVEL0;//Vuleve a configurar el juego para el nivel 1
					for (int i = 0; i < METAS; i++)
					{//Resetea las metas
						metas[i].estado = OFF;
					}
					completarFondo(arregloFondo);
					player.puntaje = 0;//Resetea el puntaje
					timer = TIEMPO;//Y el timer
					botonRa = BOTONOFF;
					pausa = FALSE;
					break;

				}

			}
			break;

		case GAMEOVER:

			if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}
			else if (joystick.x > 10 && joystick.y < -10)
			{
				botonRa = BOTON3;
			}

			if (joystick.sw == J_PRESS)
			{
				switch (botonRa)
				{
				
				case BOTON3:
				//Muestra el puntaje 
					pagina = SCORE2;
					botonRa = BOTONOFF;

					break;

				case BOTON4:
				//Resetea el juego
					pagina = MENUP;
					botonRa = BOTONOFF;
					pausa = FALSE;
					player.puntaje = 0;
					break;

				}

			}

			break;

		case SCORE2:
		//Muestra el highscore guardado
			if (joystick.x < -10 && joystick.y < -10)
			{
				botonRa = BOTON4;
			}

			if (joystick.sw == J_PRESS)
			{
				switch (botonRa)
				{

				case BOTON4:
					pagina = GAMEOVER;
					botonRa = BOTONOFF;
					break;

				}

			}


			break;
		}
		usleep(250000);
		#endif

	}
	pthread_exit(NULL);
}

//Funcion para actualizar los objetos con velocidad baja
void* velocidad1 ()
{
	while (!salir)
	{
		if(pausa == FALSE)
		{
			for (int i = 0; i < OBJETOS; i++) //Funcion update objects
			{
				if(objetos[i].velocidad == BAJA)
				{//Para objetos con velocidad baja

				//Actualiza la posicion dependiendo de que tipo de objeto es
					switch(objetos[i].tipo)
					{
					case TRONCOS5:
					//Para troncos que ocupan 5 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO5INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 4))
							{//En este caso el jugador se mueve con el movimiento del objeto
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO5INI;
						}
						break;
						
					case TRONCOS3:
					//Idem con troncos que ocupan 3 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO3INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 2))
							{
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO3INI;
						}
						break;
					
					case TORTUGAS3:
					//Idem con tortugas que ocupan 3 lugares
						
						if (objetos[i].x < MAXCOL && objetos[i].x >= TORTUGA3FIN) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 3))
							{
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else 
						{
							objetos[i].x = MAXCOL - 1;
						}
						break;
						
					case TORTUGAS2:
					//Idem con troncos que ocupan 2 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 2))
							{
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						
						break;
					//En el caso de los autos y autobuses el jugador no se mueve junto con ellos
					case AUTO1:
					case AUTO2:
					case AUTO3:
					case AUTO4:
					case AUTOBUS:
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						break;
					}
				usleep (4000*objetos[i].velocidad *nivel*plataforma);//Determina cada cuanto tiempo se actualizan
				}
			}
		}
	}
	pthread_exit(NULL);
}

//Funcion para actualizar los objetos con velocidad media
void* velocidad2 ()
{
	while (!salir)
	{
		if(pausa == FALSE)
		{
			for (int i = 0; i < OBJETOS; i++) //funcion update objects
			{
				if(objetos[i].velocidad == MEDIA)
				{//Para objetos con velocidad media

				//Actualiza la posicion dependiendo de que tipo de objeto es
					switch(objetos[i].tipo)
					{
					case TRONCOS5:
					//Para troncos que ocupan 5 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO5INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 4))
							{//El jugador se mueve junto con los objetos
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO5INI;
						}
						break;
						
					case TRONCOS3:
					//Para troncos que ocupan 3 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO3INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 2))
							{//El jugador se mueve junto con los objetos
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO3INI;
						}
						break;
					
					case TORTUGAS3:
					//Para tortugas que ocupan 3 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TORTUGA3FIN) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 3))
							{//El jugador se mueve junto con los objetos
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else 
						{
							objetos[i].x = MAXCOL - 1;
						}
						break;
						
					case TORTUGAS2:
					//Para tortugas que ocupan 2 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 2))
							{//El jugador se mueve junto con los objetos
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						
						break;
					//En el caso de los autos y autobuses el jugador no se mueve junto con los objetos
					case AUTO1:
					case AUTO2:
					case AUTO3:
					case AUTO4:
					case AUTOBUS:
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						break;
					}
				usleep (4000*objetos[i].velocidad *nivel*plataforma);
				}
			}
		}
	}
	pthread_exit(NULL);
}

//Funcion para actualizar los objetos con velocidad alta
void* velocidad3 ()
{
	while (!salir)
	{
		if(pausa == FALSE)
		{
			for (int i = 0; i < OBJETOS; i++) //funcion update objects
			{
				if(objetos[i].velocidad == ALTA)
				{	//Para objetos con velocidad alta
					switch(objetos[i].tipo)
					{
					case TRONCOS5:
					//Para troncos que ocupan 25 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO5INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 4))
							{//El jugador se mueve junto con los objetos
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO5INI;
						}
						break;
						
					case TRONCOS3:
					//Para troncos que ocupan 3 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= TRONCO3INI)
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x - 1 && player.x <= objetos[i].x + 2))
							{//El jugador se mueve junto con los objetos
								if (player.x < MAXCOL-1)
								{
									player.x++;
								}
							}
						}
						else
						{
							objetos[i].x = TRONCO3INI;
						}
						break;
					
					case TORTUGAS3:
					//Para tortugas que ocupan 3 lugares
						
						if (objetos[i].x < MAXCOL && objetos[i].x >= TORTUGA3FIN) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 3))
							{//El jugador se mueve junto con los objetos
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else 
						{
							objetos[i].x = MAXCOL - 1;
						}
						break;
						
					case TORTUGAS2:
					//Para tortugas que ocupan 2 lugares
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
							if ((player.y == objetos[i].fila) && (player.x >= objetos[i].x && player.x <= objetos[i].x + 2))
							{//El jugador se mueve junto con los objetos
								if (player.x > 0)
								{
									player.x--;
								}
							}
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						
						break;
					//En el caso de los autos y autobuses el jugador no se mueve junto con los objetos
					case AUTO1:
					case AUTO2:
					case AUTO3:
					case AUTO4:
					case AUTOBUS:
						if (objetos[i].x < MAXCOL && objetos[i].x >= 0) 
						{
							objetos[i].x += objetos[i].direccion;
						}
						else
						{
							if (objetos[i].direccion == DER)
							{
								objetos[i].x = 0;
							}
							else
							{
								objetos[i].x = MAXCOL - 1;
							}
						}
						break;
					}
				usleep (20000*objetos[i].velocidad *nivel*plataforma);
				}
			}
		}
	}
	pthread_exit(NULL);
}
