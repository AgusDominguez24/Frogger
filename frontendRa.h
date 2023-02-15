#ifndef FRONTENDRA_H
#define FRONTENDRA_H

/* LIBRERIAS */
#include <unistd.h>
#include "disdrv.h"
#include "joydrv.h"
#include "constantes.h"
#include "variablesGlobales.h"
#include "estructuras.h"


/* VARIABLES NECESARIAS */
extern joyinfo_t joystick;
extern dcoord_t display;
extern unsigned int botonRa;
extern unsigned int mostrarTimer;


/* CONSTANTES */
#define BOTONOFF 0
#define BOTON1 1
#define BOTON2 2
#define BOTON3 3
#define BOTON4 4
#define PLATAFORMA 3


/* PROTOTIPOS */

//Funcion que inicializa
int init(void);

//Funcion que imprime la pantalla
void renderizado(void);

//Funcion para cuando se finaliza el uso
void closeit(void);

#endif // FRONTENDRA_H
