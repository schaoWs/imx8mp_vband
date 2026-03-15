/* Sovereign Linker Script for i.MX8MP M7 - 2MB DDR Silo */
ENTRY(Reset_Handler)

__stack_size__ = 0x40000;
__heap_size__  = 0x40000;

HEAP_SIZE  = __heap_size__;
STACK_SIZE = __stack_size__;

MEMORY
{
  m_m7_silo (RWX) : ORIGIN = 0x80000000, LENGTH = 2M
}

SECTIONS
{
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > m_m7_silo

  .resource_table :
  {
    . = ALIGN(8);
    KEEP(*(.resource_table))
    . = ALIGN(8);
  } > m_m7_silo

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    __etext = .; /* Added this specifically for the startup script */
  } > m_m7_silo

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > m_m7_silo
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_m7_silo

  .data :
  {
    . = ALIGN(4);
    __data_start__ = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    __data_end__ = .;
  } > m_m7_silo

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > m_m7_silo

  .heap :
  {
    . = ALIGN(8);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
  } > m_m7_silo

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_m7_silo

  __StackTop   = ORIGIN(m_m7_silo) + LENGTH(m_m7_silo);
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
