SECTIONS
{
  . = 0x100000;
  .text : { *(.text) }
  . = 0x200000;
  .data : { *(.data) }
  .bss : { *(.bss) }
}