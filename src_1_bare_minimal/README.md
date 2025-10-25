# 1 Bare Minimal

Compilation command:
```sh
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c main.c -o main.o
```

Disassemble object file (contains machine code that has not been linked yet):
```sh
arm-none-eabi-objdump -d main.o
```

Disassembly output:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src$ arm-none-eabi-objdump -d main.o

main.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <main>:
   0:   b580            push    {r7, lr}
   2:   af00            add     r7, sp, #0
   4:   2300            movs    r3, #0
   6:   0018            movs    r0, r3
   8:   46bd            mov     sp, r7
   a:   bd80            pop     {r7, pc}
```

Link command to produce executable that can be flashed onto microcontroller:
```sh
arm-none-eabi-gcc main.o -o main.elf
```

We encounter the following error because `gcc` will by default link other things:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src$ arm-none-eabi-gcc main.o -o main.elf
/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/bin/ld: /usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/lib/libc.a(lib_a-exit.o): in function `exit':
/build/newlib-pB30de/newlib-3.3.0/build/arm-none-eabi/newlib/libc/stdlib/../../../../../newlib/libc/stdlib/exit.c:64: undefined reference to `_exit'
collect2: error: ld returned 1 exit status
```

Things that gcc tries to link:
- Newlib (Standard C Library)
    - Requires system calls to be implemented (e.g. _exit, _write, _fork)
    - To bypass this error we use `--specs=nosys.specs`
        - So: `arm-none-eabi-gcc --specs=nosys.specs main.o -o main.elf`

Inspecting