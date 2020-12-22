OUTPUT_FORMAT(pei-i386)    

SECTIONS{
    . = 0x40000000;

    .text :{
        *(.text)
    }

	.data ALIGN (0x1000) : {
	   start_ctors = .;
	   *(.ctor*)
	   end_ctors = .;
	   start_dtors = .;
	   *(.dtor*)
	   end_dtors = .;
	   *(.data)
	}


    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x4000) : {
        *(.data)
    }

    .bss : {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
}
