MEMORY
{
  flash (rx): ORIGIN = 0x08000000, LENGTH = 128K
  ram (rwx): ORIGIN = 0x20000000, LENGTH = 36K
}

_eram = ORIGIN(ram) + LENGTH(ram);
_stack = _eram; /* Stack starts at end of RAM and grwos downards */

/* Memory sections that hold code and data */
SECTIONS
{
  .text : ALIGN(4)    /* Linker places code in section called .text */
  {
    KEEP(*(.interrupt_vector_table))
    *(.text*)
    . = ALIGN(4);
    _etext = .;
  } > flash           /* Place in flash */

  /* Symbol to represent the initial constant values for data to be placed in flash after program code */
  _idata = LOADADDR(.data);

  .data : ALIGN(4)
  {
    _data = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > ram AT > flash  /* Place in ram and flash */

  .bss : ALIGN(4)
  {
    _bss = .;
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
    PROVIDE(_end = .);
  } > ram
}