/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"



typedef struct str_scheduler {
	jugador			siguiente_jugador;		// jugador al que le corresponde mover su zombie
  unsigned short tareaActual_A;     // proxima tarea a ejecutarse de A
  unsigned short tareaActual_B;     // proxima tarea a ejecutarse de B
  unsigned short hay_tareas_corriendo;  // bool
} scheduler;


unsigned short sched_proximo_indice();
jugador jugadorActual();
unsigned short tarea_actual_A();
unsigned short tarea_actual_B();

void cambiar_siguiente_jugador();



#endif	/* !__SCHED_H__ */
