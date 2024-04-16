#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>

#define TOGGLE(x,y) x ^= (1 << y)
#define SET(x,y) x |= (1 << y)
#define RESET(x,y) x &= ~(1 << y)

#endif