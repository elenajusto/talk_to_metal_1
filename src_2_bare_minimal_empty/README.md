# 2 Bare Minimal Empty

Compilation command:
```sh
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c main.c -o main.o
```

We initialise an empty linker script called `empty.ld`

Compiler linker instruction to not use libc and default startfiles:
```sh
arm-none-eabi-gcc -nolibc -nostartfiles -T empty.ld main.o -o main.elf
```

Object size:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_2_bare_minimal_empty$ arm-none-eabi-size main.elf
   text    data     bss     dec     hex filename
     12       0       0      12       c main.elf
```

Object dump:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_2_bare_minimal_empty$ arm-none-eabi-objdump -d main.elf

main.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <main>:
   0:   b580            push    {r7, lr}
   2:   af00            add     r7, sp, #0
   4:   2300            movs    r3, #0
   6:   0018            movs    r0, r3
   8:   46bd            mov     sp, r7
   a:   bd80            pop     {r7, pc}
```