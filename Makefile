# File:   Makefile
# Author: Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
# Date:   16 October 2025
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/system.h ../../utils/tinygl.h paddle.h ../../drivers/navswitch.h projectile.h ../../utils/pacer.h ../../drivers/ledmat.h ../../fonts/font5x7_1.h ../../utils/font.h states.h ir_transmission.h ../../drivers/ir_serial.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

paddle.o: paddle.c ../../drivers/avr/system.h ../../utils/tinygl.h ../../drivers/navswitch.h projectile.h paddle.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../utils/font.h ../../drivers/display.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h 
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/system.h ../../drivers/ledmat.h 
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

projectile.o: projectile.c ../../utils/tinygl.h ../../drivers/ledmat.h ../../utils/pacer.h ../../drivers/navswitch.h paddle.h states.h ir_transmission.h ../../utils/pacer.h projectile.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../utils/pacer.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/timer.h ../../drivers/avr/system.h 
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h ../../drivers/avr/timer0.h ../../drivers/avr/pio.h ../../drivers/avr/delay.h ../../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/system.h ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/system.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/system.h ../../drivers/avr/prescale.h
	$(CC) -c $(CFLAGS) $< -o $@

states.o: states.c ../../utils/tinygl.h ../../fonts/font5x7_1.h ../../utils/font.h ../../drivers/navswitch.h ../../utils/pacer.h states.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_transmission.o: ir_transmission.c projectile.h ../../drivers/ir_serial.h ../../drivers/navswitch.h paddle.h ../../utils/tinygl.h states.h ../../drivers/avr/system.h ir_transmission.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_serial.o: ../../drivers/ir_serial.c ../../drivers/avr/delay.h ../../drivers/ir_serial.h
	$(CC) -c $(CFLAGS) $< -o $@

ir.o: ../../drivers/ir.c ../../drivers/avr/delay.h ../../drivers/avr/system.h ../../drivers/avr/pio.h ../../drivers/ir.h 
	$(CC) -c $(CFLAGS) $< -o $@
# Link: create ELF output file from object files.
game.out: game.o system.o paddle.o tinygl.o font.o display.o ledmat.o navswitch.o projectile.o pacer.o timer.o task.o ir_uart.o usart1.o timer0.o pio.o prescale.o states.o ir_transmission.o ir_serial.o ir.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


