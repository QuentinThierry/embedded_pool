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
#include <util/twi.h>
#include <stdbool.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

extern volatile uint16_t g_value_cpt;

void	uart_init();
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printhex(uint8_t data);
void	uart_printhex_word(uint16_t data);
char	uart_rx(void);
char	uart_rtx();
void	uart_int16(uint16_t n);

void	i2c_init(void);
void	i2c_start_write(uint8_t addr);
void	i2c_start_read(uint8_t addr);
void	i2c_stop();
void	i2c_write(unsigned char data);
char	i2c_read(bool ack);

void	init_ADC();

#endif