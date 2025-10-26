/* External declaration so linker knows we defined main elsewhere (i.e. application source code) */
extern int main(void);

/* Symbols defined in linker script */
extern unsigned int _stack;
extern unsigned int _idata;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;

/* Copy initial values from flash to ram that are non-zero */
static void copy_data(void) {
    unsigned int *src_data_pointer = &_idata; /* Initial value in flash */
    unsigned int *dst_data_pointer = &_data; /* Destination address in ram */

    while (dst_data_pointer < &_edata) {
        *dst_data_pointer++ = *src_data_pointer++;
    }
}

/* Initialise the uninitialised variables that ar ezero */
static void clear_bss(void) {
    unsigned int *bss_pointer = &_bss;
    while (bss_pointer < &_ebss) {
        *bss_pointer++ = 0;
    }
}

/* ST calls this reset handler, other people call it ISR reset */
void reset_handler(void) {
    /* Prepare RAM prior to calling main */
    copy_data();
    clear_bss();

    /* Call our main C program*/
    main();

    /* In case we return from main (which should never happen) */
    while(1);
}

/* Basic hardfault handler */
void hardfault_handler(void) {

    /* For now just do while loop when encountring hard fault */
    while(1);
}

/* Define Interrupt Vector Table */
#define IVT_ARRAY_SIZE (48)

typedef void (*interrupt_table)(void);                                      /* Interrupt function */

/* Give this table a section name for the linker script to reference */
__attribute((used, section(".ivt")))

static const interrupt_table ivt[IVT_ARRAY_SIZE] = {                        /* Interrupt table as an array data type*/

    /* First: Adress of the stack's starting location */
    (interrupt_table)&_stack,

    /* Second: Address of first fucntion to run */
    reset_handler,

    /* Third: Non maskable interrupts */
    0,

    /* Fourth: Hardfault  handler */
    hardfault_handler,

    /* More interrupts that will not be defined here */
    /* Rest of them will default to 0 */
};