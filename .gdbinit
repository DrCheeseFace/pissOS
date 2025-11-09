file myos.bin

target remote :1234

hbreak kernel_main

set disassembly-flavor intel

tui enable

continue
