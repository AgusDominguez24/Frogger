#ifndef FRONTENDAL_H
#define FRONTENDAL_H

//LIBRERIAS
#include <allegro5/allegro.h> // para allegro en general
#include <allegro5/allegro_font.h> // para el tipo de fuentes
#include <allegro5/allegro_ttf.h> // para el tipo de fuentes
#include <allegro5/allegro_primitives.h> // para las primitives
#include <allegro5/allegro_image.h> // para las imagenes
#include "constantes.h" // para las constantes
#include "estructuras.h" // para las estructuras complejas que algunas funciones utilizan
#include "variablesGlobales.h" // para tenes las variables globales que usan algunas funciones

//MACROS
#define fontleftside(f,t) (560 / 2 - al_get_text_width((f),(t)) / 2)
#define fontrightside(f,t) (560 / 2 + al_get_text_width((f),(t)) / 2)

//CONSTANTES
#define PLATAFORMA 1
#define WIDTH 560
#define HEIGTH 560
#define BOTONES 5

//TYPEDEF
typedef struct
{
	int x;
	int y;
} position_t; //solo para el mouse


//VARIABLES EXTERN
extern ALLEGRO_DISPLAY* ventana;
extern ALLEGRO_EVENT evento;
extern ALLEGRO_EVENT_QUEUE* colaEventos;

extern ALLEGRO_COLOR transparente;
extern ALLEGRO_COLOR azul;
extern ALLEGRO_COLOR verde;
extern ALLEGRO_COLOR verdeoscuro;
extern ALLEGRO_COLOR negro;
extern ALLEGRO_COLOR blanco;

extern ALLEGRO_FONT* gameria[7];

extern unsigned int skinElegida;

extern position_t mouse;

extern int botonAl;


//PROTOTIPOS

// Analiza en que página se encuentra e imprime la correcta
void renderizado(void);

// Inicializa todo
int init(void);

// Destruye todo
void closeit(void);

#endif // FRONTENDAL_H
