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
extern mmu_inicializar_dir_zombi
; funcion para pintar la pantalla
extern print_screen

; funciones pic
extern resetear_pic
extern habilitar_pic

; TSS
extern tss_inicializar

extern tss_zombies

extern mappear_entorno_zombi

;Variables del juego
extern inicializar_variables_juego

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
    mov cr0,eax

    ; Inicializar tss
    call tss_inicializar
    mov ax,13<<3;
    ltr ax;:# poner en el latex ( usamos : y # )

    ; Inicializamos Zombies
    call tss_zombies

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    xchg bx, bx

    call inicializar_variables_juego
    ; Inicializar pantalla, luego de haber cargado todas las estructuras,
    ; variables del juego pero antes de habilitar el controlador de interrupciones y cargar la tarea idle
    call print_screen
    

    ; Configurar controlador de interrupciones
    call resetear_pic ; remapeamos pic (teclado a 33 y reloj a 32) a IDT
    call habilitar_pic
    sti

    ; Cargar tarea inicial
    jmp 14<<3:0
    ; (: #)
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

BITS 32
print_screen_asm:
    push eax
    push edi
    push edx

    xor eax, eax
    mov eax,0xB8000
    mov edi,0x0000
    mov edx,0x2020

    mov ecx,50
    .ciclo_col:
        push ecx
        mov [eax],di            ; movemos a la posicion del puntero el valor correp. al color negro
        add eax,2                   ; movemos el puntero 2 bytes
        mov ecx,78
        .ciclo_filas:
            mov [eax], dx       ; movemos a la posicion del puntero el valor correp. al color negro
            add eax,2
            loop .ciclo_filas
        mov [eax],di            ; movemos a la posicion del puntero el valor correp. al color negro
        add eax,2                   ; movemos el puntero 2 bytes
        pop ecx
        loop .ciclo_col

    pop edx
    pop edi
    pop eax
    ret
