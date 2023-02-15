#ifndef BACKEND_H
#define BACKEND_H

/* PROTOTIPOS */

//Funcion que prepara el fondo del juego dependiendo de en qué parte del juego te encuentres
static void completarFondo(unsigned int arregloFondo[16]);

//Funcion que actualiza el estado del jugador
static unsigned int isTheFrogDead(int x, int y);

//Funcion que comprueba si el jugador llegó a todas las metas
static unsigned int isNextLevel(void);

//Funcion que configura el nuevo nivel
static void nextLevel(void);

//Funcion que compara player.puntaje con el high score
static void compareScore(void);


#ifdef ALLEGRO
/* PROTOTIPOS FUNCIONES ALLEGRO*/


//Funcion para realizar acciones dependiendo del boton apretado
static void accion(int botonAl);

//Funcion para reconocimiento del mouse sobre una opcion del menú en el que se encuentre
static int sobreBoton(int x, int y);
#endif

#ifdef RASPI
void* playerRa(); // para el titileo del led del jugador y del timer
#endif

//Funcion para el funcionamiento del timer
void* tiempo();

//Funcion para actualizar los objetos con velocidad baja
void* velocidad1();

//Funcion para actualizar los objetos con velocidad media
void* velocidad2();

//Funcion para actualizar los objetos con velocidad alta
void* velocidad3();

//Función con la logica del funcionamiento juego
void* logic();

#endif // BACKEND_H
