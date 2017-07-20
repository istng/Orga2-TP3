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
    int r = 7;
    while(1){
    
        for(i=0;i<5;i++) {
        syscall_mover(ADE); 
        }
    
        for(i=0;i<r;i++) {
        syscall_mover(DER); 
        }
    
    
        for(i=0;i<5;i++) {
        syscall_mover(ADE); 
        }
    
    
        for(i=0;i<r;i++) {
        syscall_mover(IZQ);
        }
    
        r--;
    }

}