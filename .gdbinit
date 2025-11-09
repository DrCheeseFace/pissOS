define hook-quit
    set confirm off
end

file kernel/pissos.kernel

target remote :1234

hbreak kernel_main

set disassembly-flavor intel

tui enable

continue
