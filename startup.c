extern int main(void);

/* Symbols defined in linker script */
extern unsigned int _stack;

static void copy_data(void) {

}


void isr_reset(void) {
    main();

    // in case we return from main
    while(1);
}

/* Interrupt handler for hard faults */
void isr_hardfault(void) {
    while(1);
}

#define INTERRUPT_VECTOR_TABLE_ARRAY_SIZE (40U)
typedef void (*isr_table)(void);
__attribute((used, section(".interrupt_vector_table")))
static const isr_table interrupt_vector_table[INTERRUPT_VECTOR_TABLE_ARRAY_SIZE] =
{
    (isr_table)&_stack,
    isr_reset,
    0,
    isr_hardfault,

    // TODO: Define rest of the ISR's
};