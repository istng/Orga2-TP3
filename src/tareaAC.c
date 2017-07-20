/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h" 

void task() {
    /* Tarea */
    int i;
    for(i=0;i<13;i++)
    syscall_mover(ADE);

    int cambio = 0;
    while(1) {

        if(cambio == 0){
            for(i=0;i<13;i++){
                syscall_mover(DER);
            }
            cambio = 1;
        }
        else{
            for(i=0;i<13;i++) {
                syscall_mover(IZQ);
            }
            cambio = 0;
        }
    }

  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}