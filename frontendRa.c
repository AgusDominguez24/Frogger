#include <stdio.h>
#include "frontendRa.h"

/* VARIABLES DE RASPI */
joyinfo_t joystick;
dcoord_t display = { 0,0 }; 
unsigned int botonRa = BOTONOFF; 
unsigned int mostrarTimer = ON; // variable para el parpadeo del timer

/* PROTOTIPOS ESTATICAS */

//Funcion que muestra las distintas opciones a elegir en el menu principal
static void menup(void);

//Función con acciones a realizar al empezar una partida
static void newgame(void);

//Funcion para mostrar el highscore del juego
static void highscore(void);

//Funcion para cuando se pausa el juego
static void pausar(void);

//Funcion para el manejo del puntaje
static void score1(void);

//Función para cuando termina el juego
static void gameover(void);

//Funcion para el manejo del puntaje
static void score2(void);

//Funcion para que se muestre y configure el fondo
static void imprimirFondo(unsigned int arregloFondo[16]);

//Funcion para que se muestren los objetos
static void imprimirObjetos(void);

//Funcion para que se muestre al jugador
static void imprimirPlayer(int x, int y);

//Funcion que muestra las vidas que le quedan al jugador
static void imprimirVidas(void);

//Funcion que muestra las metas del juego
static void imprimirMetas(void);

//Funcion para poder mostrar numeros en el display de leds
static void imprimirNumero(unsigned int numero);

//Funcion para mostrar el timer
static void imprimirTimer(unsigned int timer);

//Funcion para mostrar la flecha que se ve en cada menú
static void imprimirFlecha(unsigned int flecha);

//Funcion para mostrar la flecha animada que se ve en cada menú
static void imprimirFlechaAnimada(unsigned int flecha);

//Funcion para mostrar letras en cada menú
static void imprimirLetra(char letra);


/* FUNCIONES */
int init(void)
{
	disp_init();
	disp_clear();
	disp_update();
	joy_init();

	return NONE;
}

//Funcion que dependiendo de la pagina realiza distintas acciones
void renderizado(void)
{

	disp_clear();//Limpia el display
	
	switch (pagina)
	{//Acciones a realizar dependiendo de en qué parte del juego te encuentres
	case MENUP:

		menup();

		break;

	case NEWGAME:

		newgame();

		break;
	
	case HIGHSCORE:

		highscore();

		break;

	case PAUSA:

		pausar();

		break;

	case SCORE1:

		score1();
		break;

	case GAMEOVER:

		gameover();
		break;

	case SCORE2:

		score2();
		break;
	}

	disp_update();//Actualiza los leds

	return;
}

//Funcion para cuando se finaliza el uso
void closeit(void)
{
	disp_clear();

	disp_update();

	return;
}

//Funcion que muestra las distintas opciones a elegir en el menu principal
static void menup(void)
{
	if (botonRa == BOTONOFF)
	{//Menu principal
		imprimirLetra('M');
	}

	if (botonRa == BOTON2)
	{//Para empezar una nueva partida
		imprimirLetra('N');
		imprimirFlechaAnimada(BOTON2);
	}
	else
	{
		imprimirFlecha(BOTON2);
	}

	if (botonRa == BOTON3)
	{//Muestra el highscore actual
		imprimirLetra('H');
		imprimirFlechaAnimada(BOTON3);
	}
	else
	{
		imprimirFlecha(BOTON3);
	}

	if (botonRa == BOTON4)
	{//Para salír
		imprimirLetra('E');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}

	return;
}


//Función con acciones a realizar al empezar una partida
static void newgame(void)
{
	imprimirFondo(arregloFondo);//Llama a funcion que configura el fondo
	imprimirObjetos();//Llama a funcion que muestra los objetos en el display de leds
	imprimirVidas();//Idem con las vidas
	imprimirMetas();//las metas
	imprimirPlayer(player.x, player.y);//El jugador
	imprimirTimer(timer);//Y el timer

	return;
}

//Funcion para mostrar el highscore del juego
static void highscore(void)
{
	if (botonRa == BOTONOFF)
	{//Muestra el highscore actual
		imprimirNumero(high_score);
	}

	if (botonRa == BOTON4)
	{//Para ir para atras
		imprimirLetra('B');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}


	return;
}

//Funcion para cuando se pausa el juego
static void pausar(void)
{
	if (botonRa == BOTONOFF)
	{//Menú de pausa
		imprimirLetra('P');
	}

	if (botonRa == BOTON2)
	{//Para volver al juego
		imprimirLetra('R');
		imprimirFlechaAnimada(BOTON2);
	}
	else
	{
		imprimirFlecha(BOTON2);
	}

	if (botonRa == BOTON3)
	{//Para ver el puntaje de la partida actual
		imprimirLetra('S');
		imprimirFlechaAnimada(BOTON3);
	}
	else
	{
		imprimirFlecha(BOTON3);
	}

	if (botonRa == BOTON4)
	{//Para salir al menú
		imprimirLetra('B');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}

	return;
}

//Funcion para el manejo del puntaje
static void score1(void)
{
	if (botonRa == BOTONOFF)
	{//Muestra el puntaje de la partida
		imprimirNumero(player.puntaje);
	}

	if (botonRa == BOTON4)
	{//Para ir para atras
		imprimirLetra('B');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}
	
	return;
}

//Función para cuando termina el juego
static void gameover(void)
{
	if (botonRa == BOTONOFF)
	{//Muestra una G porque terminó el juego
		imprimirLetra('G');
	}

	if (botonRa == BOTON3)
	{//Para salir
		imprimirLetra('S');
		imprimirFlechaAnimada(BOTON3);
	}
	else
	{
		imprimirFlecha(BOTON3);
	}

	if (botonRa == BOTON4)
	{
		imprimirLetra('B');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}

	return;
}

//Funcion para el manejo del puntaje
static void score2(void)
{
	if (botonRa == BOTONOFF)
	{
		imprimirNumero(player.puntaje);
	}

	if (botonRa == BOTON4)
	{
		imprimirLetra('B');
		imprimirFlechaAnimada(BOTON4);
	}
	else
	{
		imprimirFlecha(BOTON4);
	}

	return;
}

//Funcion para poder mostrar numeros en el display de leds
static void imprimirNumero(unsigned int numero)
{
	unsigned int digitosNecesarios;

	if (!(numero >= 0 && numero < 1000))
	{//Si es un numero negativo o mayor a 1000 no es valido
		printf("Numero Invalido");
		return;
	}
	else if (numero > 99)
	{//Numero de 3 digitos
		digitosNecesarios = 3;

	}
	else if (numero > 9)
	{//Numero de 2 digitos
		digitosNecesarios = 2;
	}
	else
	{//Numero de 1 digito
		digitosNecesarios = 1;
	}

	//Analiza el numero para ver que imprimir
	for (int i = 0; i < digitosNecesarios; i++)
	{
		int num = numero % 10;

		//Crea los distintos segmentos de los numeros en el display

		dcoord_t segmentoA0 = { 11 - 5 * i,2 };
		dcoord_t segmentoA1 = { 12 - 5 * i,2 };
		dcoord_t segmentoA2 = { 13 - 5 * i,2 };
		dcoord_t segmentoA3 = { 14 - 5 * i,2 };

		dcoord_t segmentoB1 = { 14 - 5 * i,3 };
		dcoord_t segmentoB2 = { 14 - 5 * i,4 };

		dcoord_t segmentoC1 = { 14 - 5 * i,6 };
		dcoord_t segmentoC2 = { 14 - 5 * i,7 };

		dcoord_t segmentoD0 = { 11 - 5 * i,8 };
		dcoord_t segmentoD1 = { 12 - 5 * i,8 };
		dcoord_t segmentoD2 = { 13 - 5 * i,8 };
		dcoord_t segmentoD3 = { 14 - 5 * i,8 };

		dcoord_t segmentoE1 = { 11 - 5 * i,6 };
		dcoord_t segmentoE2 = { 11 - 5 * i,7 };

		dcoord_t segmentoF1 = { 11 - 5 * i,3 };
		dcoord_t segmentoF2 = { 11 - 5 * i,4 };

		dcoord_t segmentoG1 = { 12 - 5 * i,5 };
		dcoord_t segmentoG2 = { 13 - 5 * i,5 };

		dcoord_t segmentoG0 = { 11 - 5 * i,5 };
		dcoord_t segmentoG3 = { 14 - 5 * i,5 };

		switch (num)
		{//Que segmento mostrar dependiendo del numero
		case 0:
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoE1, D_ON);
			disp_write(segmentoE2, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG0, D_ON);
			disp_write(segmentoG3, D_ON);

			break;

		case 1:

			disp_write(segmentoA3, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD3, D_ON);

			disp_write(segmentoG3, D_ON);


			break;

		case 2:
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoE1, D_ON);
			disp_write(segmentoE2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			break;

		case 3:
			disp_write(segmentoA0, D_ON);
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD0, D_ON);
			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);
			disp_write(segmentoG3, D_ON);

			break;

		case 4:

			disp_write(segmentoA0, D_ON);
			disp_write(segmentoA3, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD3, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			disp_write(segmentoG3, D_ON);

			break;

		case 5:
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			break;

		case 6:

			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoE1, D_ON);
			disp_write(segmentoE2, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			disp_write(segmentoG0, D_ON);

			break;

		case 7:
			disp_write(segmentoA0, D_ON);
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD3, D_ON);

			disp_write(segmentoG2, D_ON);
			disp_write(segmentoG3, D_ON);

			break;

		case 8:
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoE1, D_ON);
			disp_write(segmentoE2, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			//PROBAR
			disp_write(segmentoG0, D_ON);
			disp_write(segmentoG3, D_ON);

			break;

		case 9:
			disp_write(segmentoA1, D_ON);
			disp_write(segmentoA2, D_ON);

			disp_write(segmentoB1, D_ON);
			disp_write(segmentoB2, D_ON);

			disp_write(segmentoC1, D_ON);
			disp_write(segmentoC2, D_ON);

			disp_write(segmentoD1, D_ON);
			disp_write(segmentoD2, D_ON);

			disp_write(segmentoE2, D_ON);

			disp_write(segmentoF1, D_ON);
			disp_write(segmentoF2, D_ON);

			disp_write(segmentoG1, D_ON);
			disp_write(segmentoG2, D_ON);

			disp_write(segmentoG3, D_ON);

			break;

		default:
			break;
		}
		numero /= 10;
	}

	return;
}

//Funcion para mostrar el timer
static void imprimirTimer(unsigned int timer)
{
	unsigned int aux, i;
	dcoord_t displayTimer = { MAXCOL - 1 , MAXFIL - 1 };
	
	//El led de más a la izquierda del timer parpadea para que sea más reconocible
	if (timer > 100)
	{
		aux = 10;
	}
	else
	{
		aux = timer / 10;
	}
	for (i = aux + 1; i > 0; i--)
	{
		if (i == aux + 1)
		{
			if (mostrarTimer == ON)
			{
				displayTimer.x = MAXCOL - i;
				disp_write(displayTimer, D_ON);
			}
		}
		else
		{
			displayTimer.x = MAXCOL - i;
			disp_write(displayTimer, D_ON);
		}
	}

	return;
}

//Funcion para que se muestre y configure el fondo
static void imprimirFondo(unsigned int arregloFondo[16])
{
	int i, j;
	for ( i = 0 ; i < MAXFIL ; i++)
	{
		switch (arregloFondo[i])
		{
		case PASTOGAME:
		
			display.y = 0;
			for ( j = 0 ; j < MAXCOL ; j++)
			{
				display.x = j;

				disp_write(display, D_ON);
			}

			display.y = 1;

			for (j = 0; j < MAXCOL; j++)
			{
				display.x = j;

				disp_write(display, D_ON);
			}

			display.y = 2;

			for (j = 0; j < MAXCOL; j++)
			{
				display.x = j;

				disp_write(display, D_ON);
			}

			for (j = 1; j < MAXCOL; j+=3)
			{
				display.x = j;

				disp_write(display, D_OFF);
			}

			break;

		case PASTO:

			display.y = i;

			for (j = 0; j < MAXCOL; j++)
			{
				display.x = j;

				disp_write(display, D_ON);
			}

			break;

		default:

			break;
		}
	}
	return;
}

//Funcion para que se muestren los objetos
static void imprimirObjetos(void)
{
	int i;
	for ( i = 0 ; i < OBJETOS ; i++)
	{

		display.y = objetos[i].fila;

		switch (objetos[i].tipo)
		{
		case AUTO1:

			if (objetos[i].x < MAXCOL)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
			}

			break;

		case AUTO2:

			if (objetos[i].x < MAXCOL)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
			}

			break;

		case AUTO3:

			if (objetos[i].x >= 0)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
			}

			break;

		case AUTO4:
			
			if (objetos[i].x >= 0)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
			}

			break;

		case AUTOBUS:

			if (objetos[i].x >= 0)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
				if (objetos[i].x < MAXCOL - 1)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
				}
			}
			else if (objetos[i].x < 0)
			{
				display.x = 0;
				disp_write(display, D_ON);
			}

			break;
		
		case TRONCOS5:

			if (objetos[i].x >= 0 && objetos[i].x <= MAXCOL - 1)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
				if (objetos[i].x <= 11)
				{
					display.x = objetos[i].x+1;
					disp_write(display, D_ON);
					display.x = objetos[i].x+2;
					disp_write(display, D_ON);
					display.x = objetos[i].x+3;
					disp_write(display, D_ON);
					display.x = objetos[i].x+4;
					disp_write(display, D_ON);
				}
				else if (objetos[i].x == 12)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
					display.x = objetos[i].x + 2;
					disp_write(display, D_ON);
					display.x = objetos[i].x + 3;
					disp_write(display, D_ON);
				}
				else if (objetos[i].x == 13)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
					display.x = objetos[i].x + 2;
					disp_write(display, D_ON);
				}
				else if (objetos[i].x == 14)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
				}
			}
			else if (objetos[i].x == TRONCO5INI+3)
			{
				display.x = 3;
				disp_write(display, D_ON);
				display.x = 2;
				disp_write(display, D_ON);
				display.x = 1;
				disp_write(display, D_ON);
				display.x = 0;
				disp_write(display, D_ON);
			}
			else if (objetos[i].x == TRONCO5INI+2)
			{
				display.x = 2;
				disp_write(display, D_ON);
				display.x = 1;
				disp_write(display, D_ON);
				display.x = 0;
				disp_write(display, D_ON);
			}
			else if (objetos[i].x == TRONCO5INI+1)
			{
				display.x = 1;
				disp_write(display, D_ON); 
				display.x = 0;
				disp_write(display, D_ON);
			}
			else if (objetos[i].x == TRONCO5INI)
			{
				display.x = 0;
				disp_write(display, D_ON);
			}

			break;

		case TRONCOS3:

			if (objetos[i].x >= 0 && objetos[i].x <= MAXCOL - 1)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
				if (objetos[i].x <= 13)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
					display.x = objetos[i].x + 2;
					disp_write(display, D_ON);
				}
				else if (objetos[i].x == 14)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
				}
			}
			else if (objetos[i].x == TRONCO3INI + 1)
			{
				display.x = 1;
				disp_write(display, D_ON);
				display.x = 0;
				disp_write(display, D_ON);
			}
			else if (objetos[i].x == TRONCO3INI)
			{
				display.x = 0;
				disp_write(display, D_ON);
			}

			break;

		case TORTUGAS3:

			if (objetos[i].x >= 0 && objetos[i].x <= MAXCOL - 1)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
				if (objetos[i].x <= 13)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
					display.x = objetos[i].x + 2;
					disp_write(display, D_ON);
				}
				else if (objetos[i].x == 14)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
				}
			}
			else if (objetos[i].x == TORTUGA3FIN)
			{
				display.x = 0;
				disp_write(display, D_ON);
				display.x = 1;
				disp_write(display, D_ON);
			}
			else if (objetos[i].x == TORTUGA3FIN-1)
			{
				display.x = 0;
				disp_write(display, D_ON);
			}

			break;

		case TORTUGAS2:

			if (objetos[i].x >= 0 && objetos[i].x <= MAXCOL - 1)
			{
				display.x = objetos[i].x;
				disp_write(display, D_ON);
				if (objetos[i].x <= 14)
				{
					display.x = objetos[i].x + 1;
					disp_write(display, D_ON);
				}
			}
			else if (objetos[i].x == TORTUGA3FIN)
			{
				display.x = 0;
				disp_write(display, D_ON);
			}

			break;
		}

		

	}
	return;
}

//Funcion para que se muestre al jugador
static void imprimirPlayer(int x, int y)
{
	display.x = x;
	display.y = y;
	if(player.y >= CALLE1 && player.y <= CALLE5)
	{//Si está en la calle
		if (player.mostrar == ON)
		{
			disp_write(display, D_ON);
		}
	}
	else
	{//Si no
		if (player.mostrar == OFF)
		{
			disp_write(display, D_OFF);
		}
	}
	

	return;
}

//Funcion que muestra las vidas que le quedan al jugador
static void imprimirVidas(void)
{
	int i;
	display.y = MAXFIL-1;
	for (i = 0; i < player.vidas; i++)
	{
		display.x = i;
		disp_write(display, D_ON);
	}
	return;
}

//Funcion que muestra las metas del juego
static void imprimirMetas(void)
{
	int i;

	display.y = metas[0].fila;

	for (i = 0; i < METAS; i++)
	{
		if (metas[i].estado == ON)
		{
			display.x = metas[i].x;

			disp_write(display, D_ON);
		}
	}
	return;
}

//Funcion para mostrar la flecha que se ve en cada menú
static void imprimirFlecha(unsigned int flecha)
{
	switch (flecha)
	{
	case BOTON1:
		display.x = 1;
		display.y = 1;

		disp_write(display, D_ON);

		display.y = 2;
		disp_write(display, D_ON);

		display.y = 3;
		disp_write(display, D_ON);

		display.x = 2;
		display.y = 1;
		disp_write(display, D_ON);

		display.x = 3;
		disp_write(display, D_ON);
		break;

	case BOTON2:
		display.x = 14;
		display.y = 1;

		disp_write(display, D_ON);

		display.y = 2;
		disp_write(display, D_ON);

		display.y = 3;
		disp_write(display, D_ON);

		display.x = 12;
		display.y = 1;
		disp_write(display, D_ON);

		display.x = 13;
		disp_write(display, D_ON);
		break;

	case BOTON3:
		display.x = 14;
		display.y = 14;

		disp_write(display, D_ON);

		display.y = 12;
		disp_write(display, D_ON);

		display.y = 13;
		disp_write(display, D_ON);

		display.x = 12;
		display.y = 14;
		disp_write(display, D_ON);

		display.x = 13;
		disp_write(display, D_ON);
		break;

	case BOTON4:
		display.x = 1;
		display.y = 14;

		disp_write(display, D_ON);

		display.y = 12;
		disp_write(display, D_ON);

		display.y = 13;
		disp_write(display, D_ON);

		display.x = 2;
		display.y = 14;
		disp_write(display, D_ON);

		display.x = 3;
		disp_write(display, D_ON);
		break;

	}
	return;
}

//Funcion para mostrar la flecha animada que se ve en cada menú
static void imprimirFlechaAnimada(unsigned int flecha)
{
	static unsigned int aux = 2;

	switch (flecha)
	{
	case BOTON1:
		if (aux == 0)
		{
			display.x = 1;
			display.y = 1;

			disp_write(display, D_ON);

			display.y = 2;
			disp_write(display, D_ON);

			display.y = 3;
			disp_write(display, D_ON);

			display.x = 2;
			display.y = 1;
			disp_write(display, D_ON);

			display.x = 3;
			disp_write(display, D_ON);
		}
		else if (aux == 1)
		{
			display.x = 2;
			display.y = 2;

			disp_write(display, D_ON);

			display.y = 3;
			disp_write(display, D_ON);

			display.y = 4;
			disp_write(display, D_ON);

			display.x = 3;
			display.y = 2;
			disp_write(display, D_ON);

			display.x = 4;
			disp_write(display, D_ON);
		}
		else if (aux == 2)
		{
			display.x = 3;
			display.y = 3;

			disp_write(display, D_ON);

			display.y = 4;
			disp_write(display, D_ON);

			display.y = 5;
			disp_write(display, D_ON);

			display.x = 4;
			display.y = 3;
			disp_write(display, D_ON);

			display.x = 5;
			disp_write(display, D_ON);
		}
		break;

	case BOTON2:
		if (aux == 0)
		{
			display.x = 14;
			display.y = 1;

			disp_write(display, D_ON);

			display.y = 2;
			disp_write(display, D_ON);

			display.y = 3;
			disp_write(display, D_ON);

			display.x = 12;
			display.y = 1;
			disp_write(display, D_ON);

			display.x = 13;
			disp_write(display, D_ON);
		}
		else if (aux == 1)
		{
			display.x = 13;
			display.y = 2;

			disp_write(display, D_ON);

			display.y = 3;
			disp_write(display, D_ON);

			display.y = 4;
			disp_write(display, D_ON);

			display.x = 11;
			display.y = 2;
			disp_write(display, D_ON);

			display.x = 12;
			disp_write(display, D_ON);
		}
		else if (aux == 2)
		{
			display.x = 12;
			display.y = 3;

			disp_write(display, D_ON);

			display.y = 4;
			disp_write(display, D_ON);

			display.y = 5;
			disp_write(display, D_ON);

			display.x = 10;
			display.y = 3;
			disp_write(display, D_ON);

			display.x = 11;
			disp_write(display, D_ON);
		}
		break;

	case BOTON3:
		if (aux == 0)
		{
			display.x = 14;
			display.y = 14;

			disp_write(display, D_ON);

			display.y = 12;
			disp_write(display, D_ON);

			display.y = 13;
			disp_write(display, D_ON);

			display.x = 12;
			display.y = 14;
			disp_write(display, D_ON);

			display.x = 13;
			disp_write(display, D_ON);
		}
		else if (aux == 1)
		{
			display.x = 13;
			display.y = 13;

			disp_write(display, D_ON);

			display.y = 11;
			disp_write(display, D_ON);

			display.y = 12;
			disp_write(display, D_ON);

			display.x = 11;
			display.y = 13;
			disp_write(display, D_ON);

			display.x = 12;
			disp_write(display, D_ON);
		}
		else if (aux == 2)
		{
			display.x = 12;
			display.y = 12;

			disp_write(display, D_ON);

			display.y = 10;
			disp_write(display, D_ON);

			display.y = 11;
			disp_write(display, D_ON);

			display.x = 10;
			display.y = 12;
			disp_write(display, D_ON);

			display.x = 11;
			disp_write(display, D_ON);
		}
		break;

	case BOTON4:
		if (aux == 0)
		{
			display.x = 1;
			display.y = 14;

			disp_write(display, D_ON);

			display.y = 12;
			disp_write(display, D_ON);

			display.y = 13;
			disp_write(display, D_ON);

			display.x = 2;
			display.y = 14;
			disp_write(display, D_ON);

			display.x = 3;
			disp_write(display, D_ON);
		}
		else if (aux == 1)
		{
			display.x = 2;
			display.y = 13;

			disp_write(display, D_ON);

			display.y = 11;
			disp_write(display, D_ON);

			display.y = 12;
			disp_write(display, D_ON);

			display.x = 3;
			display.y = 13;
			disp_write(display, D_ON);

			display.x = 4;
			disp_write(display, D_ON);
		}
		else if (aux == 2)
		{
			display.x = 3;
			display.y = 12;

			disp_write(display, D_ON);

			display.y = 10;
			disp_write(display, D_ON);

			display.y = 11;
			disp_write(display, D_ON);

			display.x = 4;
			display.y = 12;
			disp_write(display, D_ON);

			display.x = 5;
			disp_write(display, D_ON);
		}
		break;

	}
	if (aux > 0)
	{
		aux--;
	}
	else
	{
		aux = 2;
	}

	usleep(250000);
	return;
}

//Funcion para mostrar letras en cada menú
static void imprimirLetra(char letra)
{
	int i;

	switch (letra)
	{
	case 'H':

		display.x = 6;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 9;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		break;

	case 'M':

		display.x = 5;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 6;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 10;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		break;

	case 'N':

		display.x = 5;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 10;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 6;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 9;

		disp_write(display, D_ON);
		break;

	case 'S':

		display.x = 9;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 9;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 10;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 10;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 10;

		disp_write(display, D_ON);

		break;

	case 'E':

		display.x = 6;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 9;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 10;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 10;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 10;

		disp_write(display, D_ON);

		break;

	case 'P':

		display.x = 5;

		for ( i = 5 ; i <= 10 ; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 6;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 9;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 10;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		break;

	case 'B':

		display.x = 6;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 7;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 9;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 10;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 10;

		disp_write(display, D_ON);

		break;

	case 'R':

		display.x = 6;

		for (i = 5; i <= 10; i++)
		{
			display.y = i;

			disp_write(display, D_ON);
		}

		display.x = 7;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 5;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 6;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 9;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 10;

		disp_write(display, D_ON);

		break;

	case 'G':

		display.x = 4;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 4;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 4;
		display.y = 4;

		disp_write(display, D_ON);

		display.x = 5;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 5;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 5;
		display.y = 4;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 4;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 4;

		disp_write(display, D_ON);

		display.x = 10;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 10;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 10;
		display.y = 4;

		disp_write(display, D_ON);

		display.x = 11;
		display.y = 2;

		disp_write(display, D_ON);

		display.x = 11;
		display.y = 3;

		disp_write(display, D_ON);

		display.x = 11;
		display.y = 4;

		disp_write(display, D_ON);


		display.x = 5;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 10;
		display.y = 7;

		disp_write(display, D_ON);

		display.x = 4;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 5;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 6;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 7;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 8;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 9;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 10;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 11;
		display.y = 8;

		disp_write(display, D_ON);

		display.x = 4;
		display.y = 9;

		disp_write(display, D_ON);

		display.x = 11;
		display.y = 9;

		disp_write(display, D_ON);

		break;
	}

	return;
}
