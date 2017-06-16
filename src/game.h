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

typedef struct info_jugador {
	unsigned int	jugador:1;				//hay dos jugadores
	unsigned int 	pos:11;					//tamaño abritrario pero suficiente
	unsigned int 	puntos:11;				//tamaño abritrario pero suficiente
	unsigned int 	zombies_usados:4; 	    //hay 8 zombies posibles por jugador
	unsigned int    zombie_seleccionado:2;  //hay 3 tipos de zombies
	unsigned int 	ultimo_zombie:3;		//indice del ultimo zombie usado
											//llegamos a 32!

} __attribute__((__packed__, aligned (4))) info_jugador;


typedef struct info_zombie {
	unsigned int	jugador:1;		//hay dos jugadores
	unsigned int 	i:10;			//filas, tamaño abritrario pero suficiente
	unsigned int 	j:11;			//columnas, tamaño abritrario pero suficiente
	unsigned int    tipo_zombie:2;  //hay 3 tipos de zombies, y 0 representa que no esta en juego
									//llegamos a 24!

} __attribute__((__packed__, aligned (4))) info_zombie;





typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(unsigned int jugador, unsigned int value);

void game_lanzar_zombi(unsigned int jugador);

void game_move_current_zombi(direccion dir);


#endif  /* !__GAME_H__ */
