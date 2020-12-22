SECTIONS
{
  . = 0x10000;
  .text : { *(.text) }
  . = 0x200;
  .data : { *(.data) }
  .bss : { *(.bss) }
}