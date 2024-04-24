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
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

void	uart_init();
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printhex(uint8_t data);
void	uart_printhex_word(uint16_t data);

void	toggle_red();
void	toggle_blue();
void	toggle_green();
void	set_red();
void	reset_red();
void	set_green();
void	reset_green();
void	set_blue();
void	reset_blue();
void	rgb_reset_all();

uint32_t	parsing();
void		reset_display();

void	uart_init();
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printhex(uint8_t data);
void	uart_printhex_word(uint16_t data);
char	uart_rx(void);
char	uart_rtx();

#endif