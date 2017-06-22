/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"

// La esctructura nodo_zombie tiene como objetivo formar una lista circular doblemente enlazada
// con los tres tipos de zombies para que resulte más sencillo cambiar el tipo de zombie
// seleccionado por el jugador
// A su vez, info_jugador va a tener entre sus miembros un puntero a esta estructura

typedef struct str_nodo_zombie {
	zombie_tipo tipo;
	char ascii;
	struct str_nodo_zombie *siguiente_nodo_zombie;
	struct str_nodo_zombie *anterior_nodo_zombie;
} nodo_zombie;


typedef struct str_info_jugador {
	jugador			jug;					//hay dos jugadores
	unsigned short 	pos;					//tamaño abritrario pero suficiente, altura del mapa, <=SIZE_H
	unsigned short 	puntos;					//tamaño abritrario pero suficiente, el primer jugador que suma 10 puntos gana
	nodo_zombie 	*zombie_seleccionado;	//hay 3 tipos de zombies
} info_jugador;


typedef struct info_zombie {
	jugador			jug;	//hay dos jugadores
	zombie_tipo		tipo;	//hay 3 tipos de zombies
	zombie_estado	estado;	//se considera ACTIVO si esta en juego e INACTIVO en caso contrario
	unsigned short 	i;		//filas, tamaño abritrario pero suficiente
	unsigned short 	j;		//columnas, tamaño abritrario pero suficiente
	unsigned int	contador_reloj;	//cantidad de veces que el zombie tuvo tiempo de ejecucion
} info_zombie;

nodo_zombie zombie_guerrero;
nodo_zombie zombie_mago;
nodo_zombie zombie_clerigo;
info_jugador A;
info_jugador B;
info_zombie zombiesA[CANT_ZOMBIS];
info_zombie zombiesB[CANT_ZOMBIS];

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(jugador jug, jugador_direccion dir);

void game_jugador_cambiar_zombie(jugador jug, jugador_zombie_lista dir);

void game_lanzar_zombi(jugador jug);

unsigned short slot_libre(jugador jug);
unsigned int hay_slot_libre(jugador jug);

info_zombie* obtener_arreglo_zombies(jugador jug);
unsigned int hay_zombies_activos(jugador jug);
unsigned short indice_siguiente_zombie_activo(jugador jug, unsigned short indice);

unsigned int llego_al_final(jugador jug, info_zombie * zombie);
unsigned int puntos(jugador jug);
unsigned int hay_otra_tarea(unsigned int i , unsigned int j);
jugador el_jugador_que_tiene_la_otra_tarea(unsigned int i , unsigned int j);
unsigned int la_otra_tarea(unsigned int i , unsigned int j);
info_zombie* obtener_zombie_actual();
void desalojar_tarea(unsigned int indice, jugador jug);
void desalojar_tarea_actual();

void game_move_current_zombi(direccion dir);


#endif  /* !__GAME_H__ */
