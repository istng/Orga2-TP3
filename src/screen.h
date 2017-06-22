/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

/* Array con los ascii del reloj */
char reloj_ascii[4];

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void print_screen();

void print_jugador(jugador jug);

void print_limpiar_pos_jugador(jugador jug);

void print_zombi(jugador jug, unsigned int i, unsigned int j);

void print_limpiar_pos_zombi(unsigned int i,unsigned int j);

void screen_anotarPuntos(jugador jug);
void screen_zombie_cadaver (unsigned int i , unsigned int j);
void print_reloj_zombie(jugador jug, unsigned int indice);
void unprint_reloj_zombie(jugador jug, unsigned int indice);
void print_debug_screen();

#endif  /* !__SCREEN_H__ */
