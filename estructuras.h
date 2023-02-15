#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

//TYPEDEF

//Estructura para el jugador
typedef struct
{
	int x; 			//Posicion en x
	int y;			//Posicion en y
	int y2;			//Se utiliza para el tema del puntaje
	int estado;		//Contiene el estado del jugador
	int facing;		//Hacia que lado mira
	unsigned int vidas;	//Cantidad de vidas
	int puntaje;		//Puntaje obtenido en la partida
	int mostrar;		//Para imprimir o no la rana
	
} player_t;


//Estructura para las metas
typedef struct
{
	int x;			//Posicion en x
	int fila;		//Fila
	int estado;		//Estado, ocupada o no
	int tipo;

} metas_t;


//Estructura para los objetos
typedef struct
{
	int x;			//Posicion en x
	int fila;		//Fila
	int estado;		//Estado
	int tipo;		//Tipo de objeto
	int direccion;		//Hacia que lado se mueve
	int velocidad;		//Con que velocidad

} objetos_t;

#endif // ESTRUCTURAS_H
