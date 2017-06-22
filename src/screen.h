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

void print_zombi(jugador jug, unsigned int indice, unsigned int i, unsigned int j);

void print_limpiar_pos_zombi(unsigned int i,unsigned int j);

void screen_anotarPuntos(jugador jug);
void screen_zombie_cadaver (unsigned int i , unsigned int j);
void print_reloj_zombie(jugador jug, unsigned int indice);
void unprint_reloj_zombie(jugador jug, unsigned int indice);
void print_debug_screen();
void print_debug_screen(unsigned int edi,unsigned int esi,unsigned int ebp,unsigned int esteNo,unsigned int ebx,   \
    unsigned int edx,unsigned int ecx,unsigned int eax,unsigned int ss,unsigned int esp, unsigned int eflags,   \
    unsigned int cs,unsigned int eip, unsigned int error_code);
void screen_guardar();
void screen_cargar();

#endif  /* !__SCREEN_H__ */
