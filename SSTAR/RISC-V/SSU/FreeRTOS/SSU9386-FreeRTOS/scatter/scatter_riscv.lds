OUTPUT_ARCH( "riscv" )
ENTRY(start)

/* Base Address RAM Memory Table 10-Mbyte on-chip RAM */
/* Total RAM available 10112K */
MEMORY {
    RAM0L (rwx)   : ORIGIN = 0x10000000, LENGTH = 0x1C0000
}

BOOT_STACK_SIZE       = 0x800;      /* BOOT mode stack      */

SECTIONS
{
    XRAM (NOLOAD) :
    {
        Image$$XRAM$$Base = .;
        . = ALIGN(0x4);
    } >RAM0L

    . = ALIGN(16);
    .text :
    {
        text_start = .;
        *start.o(.init)
        . = ALIGN(0x20);
/*
        KEEP(*(.text.entry*))
*/
        *(.text.entry*)
        *(.text*)
        *(.RoCodeOnBoot)
        *(.gnu.warning)
        *(.gnu.linkonce.t.*)
        . = ALIGN(0x4);
        Image$$ROM$$Limit = .;
        text_end = .;
    } >RAM0L

    .rodata :
    {
        _srodata = .;
        *(.rodata*)
        *(.RoDataOnBoot)
        . = ALIGN(0x4);
        _erodata = .;
    } >RAM0L

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
    } >RAM0L

    NORM_INITCALL :
    {
        Image$$NORM_INITCALL$$Base = .;
        KEEP (*(SORT(.initcall*.init)))
        Image$$NORM_INITCALL$$Limit = .;
        .   = ALIGN(0x4);
    } >RAM0L

    APPLICATION_INITCALL :
    {
        Image$$APPLICATION_INITCALL$$Base = .;
        KEEP (*(SORT(.application_initcall*.init)))
        Image$$APPLICATION_INITCALL$$Limit = .;
        .   = ALIGN(0x4);
    } >RAM0L

    XRAM0 :
    {
        Image$$XRAM0$$Base = .;
        *(kernel_memory_critical)
        *(kernel_memory_critical.rwdata)
        *(rtk_critical)
        *(rtk_critical_2)
        *(rtk_critical_2.rwdata)
        . = ALIGN(0x4);
    } >RAM0L

    .cli_cmd_list :
    {
        .   = ALIGN(0x4);
        cli_cmd_list_start = .;
        KEEP(*(SORT(.cli_cmd_list*)));
        cli_cmd_list_end = .;
    } >RAM0L

    .cam_dev_list :
    {
        . = ALIGN(0x4);
        cam_dev_list_start = .;
        KEEP(*(SORT(.cam_dev_list*)));
        cam_dev_list_end = .;
    } >RAM0L

    .data :
    {
        _sdata = .;
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
        . = ALIGN(0x4);
        _data = .;
        _edata = .;
    } >RAM0L

    RW_STATICBOOT :
    {
        *(STATICBOOT)
        . = ALIGN(0x4);
    } >RAM0L

    DEBUG_AREA :
    {
        *(DebugArea)
        *(DrvNonZI)
        . = ALIGN(0x4);
    } >RAM0L

    .bss :
    {
        . = ALIGN(0x10);
        _bss = .;
        PROVIDE(__bss_start__ = .);
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x10);
        PROVIDE(__bss_end__ = .);
        _ebss = .;
        _end = .;
        PROVIDE(end = .);
    } >RAM0L

    .sys_stack :
    {
        . = ALIGN(0x10);
        sys_stack_start = .;
        BootStack = .;
        . += BOOT_STACK_SIZE;
        sys_stack_end = .;
        BootStackStart = .;

        _sp = .;
        PROVIDE(__stack_top = .);
        PROVIDE(__freertos_irq_stack_top = .);
    } >RAM0L

    Image$$HEAP_START$$Base = .;
    Image$$HEAP_START$$Limit = 0x10000000 + 0x1C0000;
}
