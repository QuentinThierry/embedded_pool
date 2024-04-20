#ifndef MAIN_H
#define MAIN_H

#define __AVR_ATmega328P__ 1

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdbool.h>
#include <stdlib.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

#endif