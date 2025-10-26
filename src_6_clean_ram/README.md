# 6 Clean RAM
Here we modify  `startup.c` to clean up RAM of any garbage variables prior to calling `main()`.

Again compile:
```sh
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c startup.c -o startup.o
```

Again link:
```sh
arm-none-eabi-gcc -nolibc -nostartfiles -T bare.ld main.o startup.o -o main.elf
```

See the below video our proper baremetal working startup file!

<video controls src="../images/working_startup.mp4" Working Startup 1="Working Startup 1"></video>

It works :D