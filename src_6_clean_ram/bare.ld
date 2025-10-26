/* Memory layout of the STM32G071RB microcontroller */
MEMORY
{
  flash  (rx) : ORIGIN = 0x08000000, LENGTH = 128K
  ram   (rwx) : ORIGIN = 0x20000000, LENGTH = 36K
}

/* Definiton for end of RAM */
_eram = ORIGIN(ram) + LENGTH(ram);

/* Place stack at end of RAM - Expected for it to go downwards - Use same name as in startup.c */
_stack = _eram;                 

/* Define memory section that holds code and data */
SECTIONS 
{
    /* By convention linker places code in section called .text */ 
    /* On ARM the start address of .text section must be aligned in 4 byte boundary */
    .text : ALIGN(4)
    {
        /* Place Interrupt Vector Table at beginning of .text section */
        /* Keyword KEEP so linker does not discard */
        KEEP(*(.ivt))  

        /* Asterisk syntax to place any section from any object file with name that starts with .text into this setcion */
        * (.text*)

        /* Ensure end section is aligned to 4 bytes */
        . = ALIGN(4);
        _etext = .;                   /* Assign end of .text section to 4 byte alignment */

    } > flash

    /* Move our initialised variables into RAM */
    _idata = LOADADDR(.data);         /* Symbol to reference initial values of data */
    .data : ALIGN(4)
    {
      _data = .;
      *(.data*)
      . = ALIGN(4);
      _edata = .;                     /* Symbol to reference end of .data section */
    } > ram AT > flash  /* AT keywords tells linker to copy initial values from flash */

    /* Move our uninitialised (zero) variables into RAM */
    .bss : ALIGN(4)
    {
      _bss = .;
      *(.bss*)
      . = ALIGN(4);
      _ebss = .;
      PROVIDE(_end = .);
    } > ram

}