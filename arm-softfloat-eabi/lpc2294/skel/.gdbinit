target remote localhost:3333
set remotetimeout 100
monitor halt
monitor sleep 500
monitor poll
monitor soft_reset_halt
monitor sleep 500
monitor mww 0xE002C014 0x0F000924
monitor mww 0xFFE00000 0x1000348F
monitor mww 0xFFE00004 0x2000040F
monitor load_image main-extram.bin 0x81000000
monitor sleep 500
file main-extram.elf
