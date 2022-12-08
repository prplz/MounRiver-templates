OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  RAM (xrw) : ORIGIN = 0x10000000, LENGTH = 0x1C0000
}

SYS_STACK_SIZE       = 0x1000;

SECTIONS
{
  XRAM (NOLOAD) :
  {
      Image$$XRAM$$Base = .;
      . = ALIGN(0x4);
  } >RAM

  . = ALIGN(16);
  .text        :
  {
    *(.init .init.*)
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >RAM AT>RAM

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >RAM AT>RAM

  . = ALIGN(4);

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE( _eitcm = . );

  PREMAIN_INITCALL :
  {
      Image$$PREMAIN_INITCALL0$$Base = .;
      KEEP (*(SORT(.premain_initcall0.init)))
      Image$$PREMAIN_INITCALL0$$Limit = .;
      .   = ALIGN(0x4);
      Image$$PREMAIN_INITCALL1$$Base = .;
      KEEP (*(SORT(.premain_initcall1.init)))
      Image$$PREMAIN_INITCALL1$$Limit = .;
      .   = ALIGN(0x4);
  } >RAM AT>RAM

  NORM_INITCALL :
  {
      Image$$NORM_INITCALL$$Base = .;
      KEEP (*(SORT(.initcall*.init)))
      Image$$NORM_INITCALL$$Limit = .;
      .   = ALIGN(0x4);
  } >RAM AT>RAM

  APPLICATION_INITCALL :
  {
      Image$$APPLICATION_INITCALL$$Base = .;
      KEEP (*(SORT(.application_initcall*.init)))
      Image$$APPLICATION_INITCALL$$Limit = .;
      .   = ALIGN(0x4);
  } >RAM AT>RAM

  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >RAM AT>RAM

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >RAM AT>RAM

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >RAM AT>RAM

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >RAM AT>RAM

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >RAM AT>RAM

    . = ALIGN(4);
    PROVIDE( _eitcm = . );

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >RAM AT>RAM

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >RAM AT>RAM


  . = ALIGN(8);

  .data :
  {
    /**/
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)/**/
  } >RAM AT>RAM

    . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >RAM AT>RAM

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );

  .sys_stack :
  {
     . = ALIGN(0x10);
     sys_stack_start = .;
     . += SYS_STACK_SIZE;
     sys_stack_end = .;

     _sp = .;
  } >RAM AT>RAM

  Image$$HEAP_START$$Base = .;
  Image$$HEAP_START$$Limit = 0x10000000 + 0x001C0000;
}
