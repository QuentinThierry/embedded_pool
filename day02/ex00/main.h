#ifndef MAIN_H
#define MAIN_H

#define __AVR_ATmega328P__ 1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

#endif