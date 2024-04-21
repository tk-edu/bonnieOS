@define the pi 
    .cpu cortex-a72
    .fpu neon-fp-armv8
    .syntax unified 

@ Constants for assembler
@ The following are defined in /usr/include/asm-generic/fcntl.h:
@ Note that the values are specified in octal.
    .equ    O_RDWR,00000002   @ open for read/write
    .equ    O_DSYNC,00010000  @ synchronize virtual memory
    .equ    __O_SYNC,04000000 @ programming changes with
    .equ    O_SYNC,__O_SYNC|O_DSYNC @ I/O memory
@ The following are defined in /usr/include/asm-generic/mman-common.h:
    .equ    PROT_READ,0x1   @ page can be read
    .equ    PROT_WRITE,0x2  @ page can be written
    .equ    MAP_SHARED,0x01 @ share changes
@ The following are defined by me:
    .equ    PERIPH,0x3f000000   @ RPi 2 & 3 peripherals
@   .equ    PERIPH,0x20000000   @ RPi zero & 1 peripherals
    .equ    GPIO_OFFSET,0x200000  @ start of GPIO device
    .equ    O_FLAGS,O_RDWR|O_SYNC @ open file flags
    .equ    PROT_RDWR,PROT_READ|PROT_WRITE
    .equ    NO_PREF,0
    .equ    PAGE_SIZE,4096  @ Raspbian memory page
    .equ    FILE_DESCRP_ARG,0   @ file descriptor
    .equ    DEVICE_ARG,4        @ device address
    .equ    STACK_ARGS,8    @ sp already 8-byte aligned

@Constant program data
    .section .rodata
    .align 2
device: 
    .asciz "/dev/gpiomem"
fdMsg:
    .asciz "File descriptor = %i\n"
memMsg:
    .asciz "Using memory at %p\n"

@ The Program 
    .text
    .align 2
    .global main
    .type main, %function

main:
    sub sp, sp, 16      @space for saving regs
    str r4, [sp, 0]     @ save r4
    str r5, [sp, 4]     @      r5
    str fp, [sp, 8]     @      fp
    str lr, [sp, 12]    @      lr
    add fp, sp, 12      @        set frame pointer
    sub sp, sp, STACK_ARGS @ sp on 8-byte boundary 

