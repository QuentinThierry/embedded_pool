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
#include <stdbool.h>

#define TOGGLE(x,y) (x) ^= (1 << (y))
#define SET(x,y) (x) |= (1 << (y))
#define RESET(x,y) (x) &= ~(1 << (y))

void	uart_init();
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printhex(uint8_t data);
void	uart_printhex_word(uint16_t data);
char	uart_rx(void);
char	uart_rtx();


bool	parse_write(char *str);
void	exec_write(char *str);
bool	parse_read(char *str);
void	exec_read(char *str);
bool	parse_forget(char *str);
void	exec_forget(char *str);
void	parse_command();
void	reset_display();

bool eepromalloc_write(char *delim, void *buffer, uint16_t length);
bool eepromalloc_read(char *delim, void *buffer, uint16_t length);
bool eepromalloc_free(char *delim);


#endif