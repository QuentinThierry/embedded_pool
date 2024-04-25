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
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

extern volatile bool rainbowing;

void	uart_init();
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printhex(uint8_t data);
void	uart_printhex_word(uint16_t data);
char	uart_rx(void);
char	uart_rtx();
void	uart_int16(uint16_t n);

void	write_all_leds(uint32_t led1, uint32_t led2, uint32_t led3);

uint32_t	parsing(bool *error, bool *full_rainbow);

void	init_rgb();
void	wheel(uint8_t pos);

#endif