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

Inspecting memory size of the executable:
```sh
arm-none-eabi-size main.elf
```

Results in:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_1_bare_minimal$ arm-none-eabi-size main.elf
   text    data     bss     dec     hex filename
   1720    1092      28    2840     b18 main.elf
```

Disassembly of this linked executable shows:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_1_bare_minimal$ arm-none-eabi-size main.elf
   text    data     bss     dec     hex filename
   1720    1092      28    2840     b18 main.elf
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_1_bare_minimal$ arm-none-eabi-objdump -d main.elf

main.elf:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_init>:
    8000:       e1a0c00d        mov     ip, sp
    8004:       e92ddff8        push    {r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
    8008:       e24cb004        sub     fp, ip, #4
    800c:       e24bd028        sub     sp, fp, #40     ; 0x28
    8010:       e89d6ff0        ldm     sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
    8014:       e12fff1e        bx      lr

Disassembly of section .text:

00008018 <register_fini>:
    8018:       e59f3018        ldr     r3, [pc, #24]   ; 8038 <register_fini+0x20>
    801c:       e3530000        cmp     r3, #0
    8020:       012fff1e        bxeq    lr
    8024:       e92d4010        push    {r4, lr}
    8028:       e59f000c        ldr     r0, [pc, #12]   ; 803c <register_fini+0x24>
    802c:       eb000087        bl      8250 <atexit>
    8030:       e8bd4010        pop     {r4, lr}
    8034:       e12fff1e        bx      lr
    8038:       00000000        .word   0x00000000
    803c:       000082a4        .word   0x000082a4

00008040 <deregister_tm_clones>:
    8040:       e59f0018        ldr     r0, [pc, #24]   ; 8060 <deregister_tm_clones+0x20>
    8044:       e59f3018        ldr     r3, [pc, #24]   ; 8064 <deregister_tm_clones+0x24>
    8048:       e1530000        cmp     r3, r0
    804c:       012fff1e        bxeq    lr
    8050:       e59f3010        ldr     r3, [pc, #16]   ; 8068 <deregister_tm_clones+0x28>
    8054:       e3530000        cmp     r3, #0
    8058:       012fff1e        bxeq    lr
    805c:       e12fff13        bx      r3
    8060:       00018b00        .word   0x00018b00
    8064:       00018b00        .word   0x00018b00
    8068:       00000000        .word   0x00000000

0000806c <register_tm_clones>:
    806c:       e59f0024        ldr     r0, [pc, #36]   ; 8098 <register_tm_clones+0x2c>
    8070:       e59f1024        ldr     r1, [pc, #36]   ; 809c <register_tm_clones+0x30>
    8074:       e0413000        sub     r3, r1, r0
    8078:       e1a01fa3        lsr     r1, r3, #31
    807c:       e0811143        add     r1, r1, r3, asr #2
    8080:       e1b010c1        asrs    r1, r1, #1
    8084:       012fff1e        bxeq    lr
    8088:       e59f3010        ldr     r3, [pc, #16]   ; 80a0 <register_tm_clones+0x34>
    808c:       e3530000        cmp     r3, #0
    8090:       012fff1e        bxeq    lr
    8094:       e12fff13        bx      r3
    8098:       00018b00        .word   0x00018b00
    809c:       00018b00        .word   0x00018b00
    80a0:       00000000        .word   0x00000000

000080a4 <__do_global_dtors_aux>:
    80a4:       e92d4010        push    {r4, lr}
    80a8:       e59f402c        ldr     r4, [pc, #44]   ; 80dc <__do_global_dtors_aux+0x38>
    80ac:       e5d43000        ldrb    r3, [r4]
    80b0:       e3530000        cmp     r3, #0
    80b4:       1a000006        bne     80d4 <__do_global_dtors_aux+0x30>
    80b8:       ebffffe0        bl      8040 <deregister_tm_clones>
    80bc:       e59f301c        ldr     r3, [pc, #28]   ; 80e0 <__do_global_dtors_aux+0x3c>
    80c0:       e3530000        cmp     r3, #0
    80c4:       159f0018        ldrne   r0, [pc, #24]   ; 80e4 <__do_global_dtors_aux+0x40>
    80c8:       1320f000        nopne   {0}
    80cc:       e3a03001        mov     r3, #1
    80d0:       e5c43000        strb    r3, [r4]
    80d4:       e8bd4010        pop     {r4, lr}
    80d8:       e12fff1e        bx      lr
    80dc:       00018b00        .word   0x00018b00
    80e0:       00000000        .word   0x00000000
    80e4:       000086b4        .word   0x000086b4

000080e8 <frame_dummy>:
    80e8:       e59f3020        ldr     r3, [pc, #32]   ; 8110 <frame_dummy+0x28>
    80ec:       e3530000        cmp     r3, #0
    80f0:       0a000005        beq     810c <frame_dummy+0x24>
    80f4:       e92d4010        push    {r4, lr}
    80f8:       e59f1014        ldr     r1, [pc, #20]   ; 8114 <frame_dummy+0x2c>
    80fc:       e59f0014        ldr     r0, [pc, #20]   ; 8118 <frame_dummy+0x30>
    8100:       e320f000        nop     {0}
    8104:       e8bd4010        pop     {r4, lr}
    8108:       eaffffd7        b       806c <register_tm_clones>
    810c:       eaffffd6        b       806c <register_tm_clones>
    8110:       00000000        .word   0x00000000
    8114:       00018b04        .word   0x00018b04
    8118:       000086b4        .word   0x000086b4

0000811c <_stack_init>:
    811c:       e10f4000        mrs     r4, CPSR
    8120:       e314000f        tst     r4, #15
    8124:       0a00001c        beq     819c <_stack_init+0x80>
    8128:       e1a0300d        mov     r3, sp
    812c:       e3a010d1        mov     r1, #209        ; 0xd1
    8130:       e121f001        msr     CPSR_c, r1
    8134:       e1a0d003        mov     sp, r3
    8138:       e24daa01        sub     sl, sp, #4096   ; 0x1000
    813c:       e1a0300a        mov     r3, sl
    8140:       e3a010d7        mov     r1, #215        ; 0xd7
    8144:       e121f001        msr     CPSR_c, r1
    8148:       e1a0d003        mov     sp, r3
    814c:       e2433a01        sub     r3, r3, #4096   ; 0x1000
    8150:       e3a010db        mov     r1, #219        ; 0xdb
    8154:       e121f001        msr     CPSR_c, r1
    8158:       e1a0d003        mov     sp, r3
    815c:       e2433a01        sub     r3, r3, #4096   ; 0x1000
    8160:       e3a010d2        mov     r1, #210        ; 0xd2
    8164:       e121f001        msr     CPSR_c, r1
    8168:       e1a0d003        mov     sp, r3
    816c:       e2433a02        sub     r3, r3, #8192   ; 0x2000
    8170:       e3a010d3        mov     r1, #211        ; 0xd3
    8174:       e121f001        msr     CPSR_c, r1
    8178:       e1a0d003        mov     sp, r3
    817c:       e2433902        sub     r3, r3, #32768  ; 0x8000
    8180:       e3c330ff        bic     r3, r3, #255    ; 0xff
    8184:       e3c33cff        bic     r3, r3, #65280  ; 0xff00
    8188:       e3a010df        mov     r1, #223        ; 0xdf
    818c:       e121f001        msr     CPSR_c, r1
    8190:       e1a0d003        mov     sp, r3
    8194:       e38440c0        orr     r4, r4, #192    ; 0xc0
    8198:       e121f004        msr     CPSR_c, r4
    819c:       e243a801        sub     sl, r3, #65536  ; 0x10000
    81a0:       e12fff1e        bx      lr

000081a4 <_mainCRTStartup>:
    81a4:       e59f3088        ldr     r3, [pc, #136]  ; 8234 <_mainCRTStartup+0x90>
    81a8:       e3530000        cmp     r3, #0
    81ac:       059f3074        ldreq   r3, [pc, #116]  ; 8228 <_mainCRTStartup+0x84>
    81b0:       e1a0d003        mov     sp, r3
    81b4:       ebffffd8        bl      811c <_stack_init>
    81b8:       e3b01000        movs    r1, #0
    81bc:       e1a0b001        mov     fp, r1
    81c0:       e1a07001        mov     r7, r1
    81c4:       e59f006c        ldr     r0, [pc, #108]  ; 8238 <_mainCRTStartup+0x94>
    81c8:       e59f206c        ldr     r2, [pc, #108]  ; 823c <_mainCRTStartup+0x98>
    81cc:       e0522000        subs    r2, r2, r0
    81d0:       eb000066        bl      8370 <memset>
    81d4:       e59f3050        ldr     r3, [pc, #80]   ; 822c <_mainCRTStartup+0x88>
    81d8:       e3530000        cmp     r3, #0
    81dc:       0a000001        beq     81e8 <_mainCRTStartup+0x44>
    81e0:       e1a0e00f        mov     lr, pc
    81e4:       e1a0f003        mov     pc, r3
    81e8:       e59f3040        ldr     r3, [pc, #64]   ; 8230 <_mainCRTStartup+0x8c>
    81ec:       e3530000        cmp     r3, #0
    81f0:       0a000001        beq     81fc <_mainCRTStartup+0x58>
    81f4:       e1a0e00f        mov     lr, pc
    81f8:       e1a0f003        mov     pc, r3
    81fc:       e3b00000        movs    r0, #0
    8200:       e3b01000        movs    r1, #0
    8204:       e1b04000        movs    r4, r0
    8208:       e1b05001        movs    r5, r1
    820c:       e59f002c        ldr     r0, [pc, #44]   ; 8240 <_mainCRTStartup+0x9c>
    8210:       eb00000e        bl      8250 <atexit>
    8214:       eb000033        bl      82e8 <__libc_init_array>
    8218:       e1b00004        movs    r0, r4
    821c:       e1b01005        movs    r1, r5
    8220:       eb000104        bl      8638 <__main_from_arm>
    8224:       eb000011        bl      8270 <exit>
    8228:       00080000        .word   0x00080000
        ...
    8238:       00018b00        .word   0x00018b00
    823c:       00018b1c        .word   0x00018b1c
    8240:       000082a4        .word   0x000082a4

00008244 <main>:
    8244:       b580            push    {r7, lr}
    8246:       af00            add     r7, sp, #0
    8248:       2300            movs    r3, #0
    824a:       0018            movs    r0, r3
    824c:       46bd            mov     sp, r7
    824e:       bd80            pop     {r7, pc}

00008250 <atexit>:
    8250:       e3a03000        mov     r3, #0
    8254:       e92d4010        push    {r4, lr}
    8258:       e1a01000        mov     r1, r0
    825c:       e1a02003        mov     r2, r3
    8260:       e1a00003        mov     r0, r3
    8264:       eb000088        bl      848c <__register_exitproc>
    8268:       e8bd4010        pop     {r4, lr}
    826c:       e12fff1e        bx      lr

00008270 <exit>:
    8270:       e92d4010        push    {r4, lr}
    8274:       e3a01000        mov     r1, #0
    8278:       e1a04000        mov     r4, r0
    827c:       eb0000a5        bl      8518 <__call_exitprocs>
    8280:       e59f3018        ldr     r3, [pc, #24]   ; 82a0 <exit+0x30>
    8284:       e5930000        ldr     r0, [r3]
    8288:       e590303c        ldr     r3, [r0, #60]   ; 0x3c
    828c:       e3530000        cmp     r3, #0
    8290:       11a0e00f        movne   lr, pc
    8294:       112fff13        bxne    r3
    8298:       e1a00004        mov     r0, r4
    829c:       eb0000e3        bl      8630 <_exit>
    82a0:       00008684        .word   0x00008684

000082a4 <__libc_fini_array>:
    82a4:       e92d4070        push    {r4, r5, r6, lr}
    82a8:       e59f5030        ldr     r5, [pc, #48]   ; 82e0 <__libc_fini_array+0x3c>
    82ac:       e59f4030        ldr     r4, [pc, #48]   ; 82e4 <__libc_fini_array+0x40>
    82b0:       e0454004        sub     r4, r5, r4
    82b4:       e1b04144        asrs    r4, r4, #2
    82b8:       0a000005        beq     82d4 <__libc_fini_array+0x30>
    82bc:       e5353004        ldr     r3, [r5, #-4]!
    82c0:       e2444001        sub     r4, r4, #1
    82c4:       e1a0e00f        mov     lr, pc
    82c8:       e12fff13        bx      r3
    82cc:       e3540000        cmp     r4, #0
    82d0:       1afffff9        bne     82bc <__libc_fini_array+0x18>
    82d4:       eb0000db        bl      8648 <_fini>
    82d8:       e8bd4070        pop     {r4, r5, r6, lr}
    82dc:       e12fff1e        bx      lr
    82e0:       000186c4        .word   0x000186c4
    82e4:       000186c0        .word   0x000186c0

000082e8 <__libc_init_array>:
    82e8:       e92d4070        push    {r4, r5, r6, lr}
    82ec:       e59f606c        ldr     r6, [pc, #108]  ; 8360 <__libc_init_array+0x78>
    82f0:       e59f506c        ldr     r5, [pc, #108]  ; 8364 <__libc_init_array+0x7c>
    82f4:       e0466005        sub     r6, r6, r5
    82f8:       e1b06146        asrs    r6, r6, #2
    82fc:       0a000007        beq     8320 <__libc_init_array+0x38>
    8300:       e3a04000        mov     r4, #0
    8304:       e2455004        sub     r5, r5, #4
    8308:       e5b53004        ldr     r3, [r5, #4]!
    830c:       e2844001        add     r4, r4, #1
    8310:       e1a0e00f        mov     lr, pc
    8314:       e12fff13        bx      r3
    8318:       e1560004        cmp     r6, r4
    831c:       1afffff9        bne     8308 <__libc_init_array+0x20>
    8320:       e59f6040        ldr     r6, [pc, #64]   ; 8368 <__libc_init_array+0x80>
    8324:       e59f5040        ldr     r5, [pc, #64]   ; 836c <__libc_init_array+0x84>
    8328:       e0466005        sub     r6, r6, r5
    832c:       ebffff33        bl      8000 <_init>
    8330:       e1b06146        asrs    r6, r6, #2
    8334:       0a000007        beq     8358 <__libc_init_array+0x70>
    8338:       e3a04000        mov     r4, #0
    833c:       e2455004        sub     r5, r5, #4
    8340:       e5b53004        ldr     r3, [r5, #4]!
    8344:       e2844001        add     r4, r4, #1
    8348:       e1a0e00f        mov     lr, pc
    834c:       e12fff13        bx      r3
    8350:       e1560004        cmp     r6, r4
    8354:       1afffff9        bne     8340 <__libc_init_array+0x58>
    8358:       e8bd4070        pop     {r4, r5, r6, lr}
    835c:       e12fff1e        bx      lr
    8360:       000186b8        .word   0x000186b8
    8364:       000186b8        .word   0x000186b8
    8368:       000186c0        .word   0x000186c0
    836c:       000186b8        .word   0x000186b8

00008370 <memset>:
    8370:       e3100003        tst     r0, #3
    8374:       0a000040        beq     847c <memset+0x10c>
    8378:       e3520000        cmp     r2, #0
    837c:       e2422001        sub     r2, r2, #1
    8380:       012fff1e        bxeq    lr
    8384:       e1a03000        mov     r3, r0
    8388:       e201c0ff        and     ip, r1, #255    ; 0xff
    838c:       ea000002        b       839c <memset+0x2c>
    8390:       e2422001        sub     r2, r2, #1
    8394:       e3720001        cmn     r2, #1
    8398:       012fff1e        bxeq    lr
    839c:       e4c3c001        strb    ip, [r3], #1
    83a0:       e3130003        tst     r3, #3
    83a4:       1afffff9        bne     8390 <memset+0x20>
    83a8:       e3520003        cmp     r2, #3
    83ac:       9a000028        bls     8454 <memset+0xe4>
    83b0:       e92d4030        push    {r4, r5, lr}
    83b4:       e201e0ff        and     lr, r1, #255    ; 0xff
    83b8:       e18ee40e        orr     lr, lr, lr, lsl #8
    83bc:       e352000f        cmp     r2, #15
    83c0:       e18ee80e        orr     lr, lr, lr, lsl #16
    83c4:       9a00002e        bls     8484 <memset+0x114>
    83c8:       e242c010        sub     ip, r2, #16
    83cc:       e3cc400f        bic     r4, ip, #15
    83d0:       e2835020        add     r5, r3, #32
    83d4:       e0855004        add     r5, r5, r4
    83d8:       e1a0422c        lsr     r4, ip, #4
    83dc:       e283c010        add     ip, r3, #16
    83e0:       e50ce010        str     lr, [ip, #-16]
    83e4:       e50ce00c        str     lr, [ip, #-12]
    83e8:       e50ce008        str     lr, [ip, #-8]
    83ec:       e50ce004        str     lr, [ip, #-4]
    83f0:       e28cc010        add     ip, ip, #16
    83f4:       e15c0005        cmp     ip, r5
    83f8:       1afffff8        bne     83e0 <memset+0x70>
    83fc:       e284c001        add     ip, r4, #1
    8400:       e312000c        tst     r2, #12
    8404:       e083c20c        add     ip, r3, ip, lsl #4
    8408:       e202200f        and     r2, r2, #15
    840c:       0a000018        beq     8474 <memset+0x104>
    8410:       e2423004        sub     r3, r2, #4
    8414:       e3c33003        bic     r3, r3, #3
    8418:       e2833004        add     r3, r3, #4
    841c:       e08c3003        add     r3, ip, r3
    8420:       e48ce004        str     lr, [ip], #4
    8424:       e153000c        cmp     r3, ip
    8428:       1afffffc        bne     8420 <memset+0xb0>
    842c:       e2022003        and     r2, r2, #3
    8430:       e3520000        cmp     r2, #0
    8434:       0a000004        beq     844c <memset+0xdc>
    8438:       e20110ff        and     r1, r1, #255    ; 0xff
    843c:       e0832002        add     r2, r3, r2
    8440:       e4c31001        strb    r1, [r3], #1
    8444:       e1520003        cmp     r2, r3
    8448:       1afffffc        bne     8440 <memset+0xd0>
    844c:       e8bd4030        pop     {r4, r5, lr}
    8450:       e12fff1e        bx      lr
    8454:       e3520000        cmp     r2, #0
    8458:       012fff1e        bxeq    lr
    845c:       e20110ff        and     r1, r1, #255    ; 0xff
    8460:       e0832002        add     r2, r3, r2
    8464:       e4c31001        strb    r1, [r3], #1
    8468:       e1520003        cmp     r2, r3
    846c:       1afffffc        bne     8464 <memset+0xf4>
    8470:       e12fff1e        bx      lr
    8474:       e1a0300c        mov     r3, ip
    8478:       eaffffec        b       8430 <memset+0xc0>
    847c:       e1a03000        mov     r3, r0
    8480:       eaffffc8        b       83a8 <memset+0x38>
    8484:       e1a0c003        mov     ip, r3
    8488:       eaffffe0        b       8410 <memset+0xa0>

0000848c <__register_exitproc>:
    848c:       e59fc080        ldr     ip, [pc, #128]  ; 8514 <__register_exitproc+0x88>
    8490:       e92d4010        push    {r4, lr}
    8494:       e59ce000        ldr     lr, [ip]
    8498:       e59ec148        ldr     ip, [lr, #328]  ; 0x148
    849c:       e35c0000        cmp     ip, #0
    84a0:       028ecf53        addeq   ip, lr, #332    ; 0x14c
    84a4:       058ec148        streq   ip, [lr, #328]  ; 0x148
    84a8:       e59ce004        ldr     lr, [ip, #4]
    84ac:       e35e001f        cmp     lr, #31
    84b0:       ca000015        bgt     850c <__register_exitproc+0x80>
    84b4:       e3500000        cmp     r0, #0
    84b8:       1a000006        bne     84d8 <__register_exitproc+0x4c>
    84bc:       e3a00000        mov     r0, #0
    84c0:       e28e3001        add     r3, lr, #1
    84c4:       e28ee002        add     lr, lr, #2
    84c8:       e58c3004        str     r3, [ip, #4]
    84cc:       e78c110e        str     r1, [ip, lr, lsl #2]
    84d0:       e8bd4010        pop     {r4, lr}
    84d4:       e12fff1e        bx      lr
    84d8:       e3500002        cmp     r0, #2
    84dc:       e3a00001        mov     r0, #1
    84e0:       e08c410e        add     r4, ip, lr, lsl #2
    84e4:       e5842088        str     r2, [r4, #136]  ; 0x88
    84e8:       e1a02e10        lsl     r2, r0, lr
    84ec:       e59c0188        ldr     r0, [ip, #392]  ; 0x188
    84f0:       e1800002        orr     r0, r0, r2
    84f4:       e58c0188        str     r0, [ip, #392]  ; 0x188
    84f8:       e5843108        str     r3, [r4, #264]  ; 0x108
    84fc:       059c318c        ldreq   r3, [ip, #396]  ; 0x18c
    8500:       01832002        orreq   r2, r3, r2
    8504:       058c218c        streq   r2, [ip, #396]  ; 0x18c
    8508:       eaffffeb        b       84bc <__register_exitproc+0x30>
    850c:       e3e00000        mvn     r0, #0
    8510:       eaffffee        b       84d0 <__register_exitproc+0x44>
    8514:       00008684        .word   0x00008684

00008518 <__call_exitprocs>:
    8518:       e59f310c        ldr     r3, [pc, #268]  ; 862c <__call_exitprocs+0x114>
    851c:       e92d4ff0        push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    8520:       e5937000        ldr     r7, [r3]
    8524:       e5976148        ldr     r6, [r7, #328]  ; 0x148
    8528:       e24dd00c        sub     sp, sp, #12
    852c:       e3560000        cmp     r6, #0
    8530:       e58d0004        str     r0, [sp, #4]
    8534:       0a000029        beq     85e0 <__call_exitprocs+0xc8>
    8538:       e1a0b001        mov     fp, r1
    853c:       e3a08001        mov     r8, #1
    8540:       e3a09000        mov     r9, #0
    8544:       e5964004        ldr     r4, [r6, #4]
    8548:       e2545001        subs    r5, r4, #1
    854c:       4a000023        bmi     85e0 <__call_exitprocs+0xc8>
    8550:       e2844001        add     r4, r4, #1
    8554:       e0864104        add     r4, r6, r4, lsl #2
    8558:       ea000003        b       856c <__call_exitprocs+0x54>
    855c:       e2455001        sub     r5, r5, #1
    8560:       e3750001        cmn     r5, #1
    8564:       e2444004        sub     r4, r4, #4
    8568:       0a00001c        beq     85e0 <__call_exitprocs+0xc8>
    856c:       e35b0000        cmp     fp, #0
    8570:       0a000002        beq     8580 <__call_exitprocs+0x68>
    8574:       e5943100        ldr     r3, [r4, #256]  ; 0x100
    8578:       e153000b        cmp     r3, fp
    857c:       1afffff6        bne     855c <__call_exitprocs+0x44>
    8580:       e5963004        ldr     r3, [r6, #4]
    8584:       e5942000        ldr     r2, [r4]
    8588:       e2433001        sub     r3, r3, #1
    858c:       e1530005        cmp     r3, r5
    8590:       05865004        streq   r5, [r6, #4]
    8594:       15849000        strne   r9, [r4]
    8598:       e3520000        cmp     r2, #0
    859c:       0affffee        beq     855c <__call_exitprocs+0x44>
    85a0:       e5963188        ldr     r3, [r6, #392]  ; 0x188
    85a4:       e1a01518        lsl     r1, r8, r5
    85a8:       e1110003        tst     r1, r3
    85ac:       e596a004        ldr     sl, [r6, #4]
    85b0:       1a00000d        bne     85ec <__call_exitprocs+0xd4>
    85b4:       e1a0e00f        mov     lr, pc
    85b8:       e12fff12        bx      r2
    85bc:       e5962004        ldr     r2, [r6, #4]
    85c0:       e152000a        cmp     r2, sl
    85c4:       1a000012        bne     8614 <__call_exitprocs+0xfc>
    85c8:       e5973148        ldr     r3, [r7, #328]  ; 0x148
    85cc:       e1530006        cmp     r3, r6
    85d0:       0affffe1        beq     855c <__call_exitprocs+0x44>
    85d4:       e1a06003        mov     r6, r3
    85d8:       e3560000        cmp     r6, #0
    85dc:       1affffd8        bne     8544 <__call_exitprocs+0x2c>
    85e0:       e28dd00c        add     sp, sp, #12
    85e4:       e8bd4ff0        pop     {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    85e8:       e12fff1e        bx      lr
    85ec:       e596018c        ldr     r0, [r6, #396]  ; 0x18c
    85f0:       e1110000        tst     r1, r0
    85f4:       1a000008        bne     861c <__call_exitprocs+0x104>
    85f8:       e5941080        ldr     r1, [r4, #128]  ; 0x80
    85fc:       e59d0004        ldr     r0, [sp, #4]
    8600:       e1a0e00f        mov     lr, pc
    8604:       e12fff12        bx      r2
    8608:       e5962004        ldr     r2, [r6, #4]
    860c:       e152000a        cmp     r2, sl
    8610:       0affffec        beq     85c8 <__call_exitprocs+0xb0>
    8614:       e5976148        ldr     r6, [r7, #328]  ; 0x148
    8618:       eaffffee        b       85d8 <__call_exitprocs+0xc0>
    861c:       e5940080        ldr     r0, [r4, #128]  ; 0x80
    8620:       e1a0e00f        mov     lr, pc
    8624:       e12fff12        bx      r2
    8628:       eaffffe3        b       85bc <__call_exitprocs+0xa4>
    862c:       00008684        .word   0x00008684

00008630 <_exit>:
    8630:       eafffffe        b       8630 <_exit>
    8634:       00000000        andeq   r0, r0, r0

00008638 <__main_from_arm>:
    8638:       e59fc000        ldr     ip, [pc]        ; 8640 <__main_from_arm+0x8>
    863c:       e12fff1c        bx      ip
    8640:       00008245        .word   0x00008245
    8644:       00000000        .word   0x00000000

Disassembly of section .fini:

00008648 <_fini>:
    8648:       e1a0c00d        mov     ip, sp
    864c:       e92ddff8        push    {r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
    8650:       e24cb004        sub     fp, ip, #4
    8654:       e24bd028        sub     sp, fp, #40     ; 0x28
    8658:       e89d6ff0        ldm     sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
    865c:       e12fff1e        bx      lr
```

The aboe output shows that the default gcc linker script has linked in lots of stuff in addition to our main function. Some notable things it linked are:
- Standard library function (memset)
- Runtime library code (crt*.o)

We also see our instructions are at 0x8000 rather then at 0x8000000 whic his where the STM32 flash memory is located. This is again due to the default linker script that gcc uses.

To dump the default linker script that gcc uses:
```sh
arm-none-eabi-gcc --specs=nosys.specs -Wl, --verbose main.o -o main.elf
```

Hmmm I get this issue though:
```sh
(base) elena@elenadesktop:~/repos/talk_to_metal_1/src_1_bare_minimal$ arm-none-eabi-gcc --specs=nosys.specs -Wl, --verbose main.o -o main.elf
Using built-in specs.
Reading specs from /usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/lib/nosys.specs
rename spec link_gcc_c_sequence to nosys_link_gcc_c_sequence
COLLECT_GCC=arm-none-eabi-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/arm-none-eabi/10.3.1/lto-wrapper
Target: arm-none-eabi
Configured with: ../configure --build=x86_64-linux-gnu --prefix=/usr --includedir='/usr/lib/include' --mandir='/usr/lib/share/man' --infodir='/usr/lib/share/info' --sysconfdir=/etc --localstatedir=/var --disable-option-checking --disable-silent-rules --libdir='/usr/lib/lib/x86_64-linux-gnu' --libexecdir='/usr/lib/lib/x86_64-linux-gnu' --disable-maintainer-mode --disable-dependency-tracking --mandir=/usr/share/man --enable-languages=c,c++,lto --enable-multilib --disable-decimal-float --disable-libffi --disable-libgomp --disable-libmudflap --disable-libquadmath --disable-libssp --disable-libstdcxx-pch --disable-nls --disable-shared --disable-threads --enable-tls --build=x86_64-linux-gnu --target=arm-none-eabi --with-system-zlib --with-gnu-as --with-gnu-ld --with-pkgversion=15:10.3-2021.07-4 --without-included-gettext --prefix=/usr/lib --infodir=/usr/share/doc/gcc-arm-none-eabi/info --htmldir=/usr/share/doc/gcc-arm-none-eabi/html --pdfdir=/usr/share/doc/gcc-arm-none-eabi/pdf --bindir=/usr/bin --libexecdir=/usr/lib --libdir=/usr/lib --disable-libstdc++-v3 --host=x86_64-linux-gnu --with-headers=no --without-newlib --with-multilib-list=rmprofile,aprofile CFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' CPPFLAGS='-Wdate-time -D_FORTIFY_SOURCE=2' CXXFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' DFLAGS=-frelease FCFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' FFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' GCJFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -fstack-protector-strong' LDFLAGS='-Wl,-Bsymbolic-functions -flto=auto -Wl,-z,relro' OBJCFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' OBJCXXFLAGS='-g -O2 -ffile-prefix-map=/build/gcc-arm-none-eabi-hYfgK4/gcc-arm-none-eabi-10.3-2021.07=. -flto=auto -ffat-lto-objects -fstack-protector-strong' INHIBIT_LIBC_CFLAGS=-DUSE_TM_CLONE_REGISTRY=0 AR_FOR_TARGET=arm-none-eabi-ar AS_FOR_TARGET=arm-none-eabi-as LD_FOR_TARGET=arm-none-eabi-ld NM_FOR_TARGET=arm-none-eabi-nm OBJDUMP_FOR_TARGET=arm-none-eabi-objdump RANLIB_FOR_TARGET=arm-none-eabi-ranlib READELF_FOR_TARGET=arm-none-eabi-readelf STRIP_FOR_TARGET=arm-none-eabi-strip SED=/bin/sed SHELL=/bin/sh BASH=/bin/bash CONFIG_SHELL=/bin/bash
Thread model: single
Supported LTO compression algorithms: zlib
gcc version 10.3.1 20210621 (release) (15:10.3-2021.07-4) 
COMPILER_PATH=/usr/lib/gcc/arm-none-eabi/10.3.1/:/usr/lib/gcc/arm-none-eabi/10.3.1/:/usr/lib/gcc/arm-none-eabi/:/usr/lib/gcc/arm-none-eabi/10.3.1/:/usr/lib/gcc/arm-none-eabi/:/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/bin/
LIBRARY_PATH=/usr/lib/gcc/arm-none-eabi/10.3.1/:/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/lib/
COLLECT_GCC_OPTIONS='-specs=nosys.specs' '-v' '-o' 'main.elf' '-mcpu=arm7tdmi' '-mfloat-abi=soft' '-marm' '-mlibarch=armv4t' '-march=armv4t'
 /usr/lib/gcc/arm-none-eabi/10.3.1/collect2 -plugin /usr/lib/gcc/arm-none-eabi/10.3.1/liblto_plugin.so -plugin-opt=/usr/lib/gcc/arm-none-eabi/10.3.1/lto-wrapper -plugin-opt=-fresolution=/tmp/cceo8qES.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lnosys -X -o main.elf /usr/lib/gcc/arm-none-eabi/10.3.1/crti.o /usr/lib/gcc/arm-none-eabi/10.3.1/crtbegin.o /usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/lib/crt0.o -L/usr/lib/gcc/arm-none-eabi/10.3.1 -L/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/lib "" main.o --start-group -lgcc -lc --end-group --start-group -lgcc -lc -lnosys --end-group /usr/lib/gcc/arm-none-eabi/10.3.1/crtend.o /usr/lib/gcc/arm-none-eabi/10.3.1/crtn.o
/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/bin/ld: cannot find : No such file or directory
collect2: error: ld returned 1 exit status
```

Nevertheless, the ARM GCC default linking behaviour is as follows:
- Newlib (Standard C library)
    - System calls (_exit_, _write_, _fork_...)
- Startfiles/C runtime library (crt*.0)
- Default linker script