; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

;GDT:
extern GDT_DESC

;IDT:
extern IDT_DESC
; funcion inicializar IDT:
extern idt_inicializar
; funcion incializar MMU, mapear, desmapear paginas
extern mmu_inicializar_dir_kernel
extern mmu_mappear_pagina
extern mmu_unmapear_pagina
; funcion para pintar la pantalla
extern print_screen

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; Habilitar A20
    call habilitar_A20
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1
    mov cr0,eax

    ; Saltar a modo protegido
    jmp 8<<3:modo_protegido
      

    ; |    indice     | 000 (3bits) |
    ; |     0x08      | 000 (3bits) |
    ; 01010 0000

BITS 32
modo_protegido:

    ; Establecer selectores de segmentos
    mov ax,10<<3
    mov ds,ax
    mov ss,ax
    mov es,ax
    mov gs,ax
    mov ax,12<<3
    mov fs,ax

    ; Establecer la base de la pila
    mov ebp,0x27000
    mov esp,0x27000

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    xchg bx, bx ; Breakpoint
    call print_screen

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    xor eax,eax
    mov eax,0x27000 ; |  dir_pde(20 bits altos) | 000000000000 |
    mov cr3, eax

    ; Habilitar paginacion
    mov eax,cr0
    or eax,0x80000000
    xchg bx, bx ; Breakpoint
    mov cr0,eax

    push 0x9832000  ; test para ver si funciona maper_pagina (usar comando info tab)
    push 0x27000
    push 0x5989000
    xchg bx, bx ; Breakpoint
    call mmu_mappear_pagina
    xchg bx, bx ; Breakpoint
    add esp, 12     ; "pop" de los parametros
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm" 
