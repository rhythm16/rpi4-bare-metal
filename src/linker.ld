SECTIONS
{
    .text.boot : { *(.text.boot) }
    .text : { main_build/*(.text) trace_build/*(.text) }
    .rodata : { main_build/*(.rodata) trace_build/*(.rodata) }
    .data : { main_build/*(.data) trace_build/*(.data) }
    . = ALIGN(0x8);
    __start_patchable_functions = .;
    .patchable_function_entries : {
        KEEP(*(__patchable_function_entries))
    }
    __stop_patchable_functions = .;
    . = ALIGN(0x8);
    ksyms = .;
    symbols : {
        KEEP(*(_symbols))
    }
    . = ALIGN(0x8);
    bss_begin = .;
    .bss : { main_build/*(.bss*) trace_build/*(.bss*) }
    bss_end = .;
    . = ALIGN(0x1000);
    id_pg_dir = .;
    .data.id_pg_dir : { . += (3 * (1 << 12)); }
    high_pg_dir = .;
    .data.high_pg_dir : { . += (6 * (1 << 12)); }
    user_start = .;
    .text.user : { user_build/*(.text) }
    .rodata.user : { user_build/*(.rodata) }
    .data.user : { user_build/*(.data) }
    .bss.user : { user_build/*(.bss) }
    . = ALIGN(0x1000);
    user_end = .;
}
