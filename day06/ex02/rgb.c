#include "main.h"

void	toggle_red()
{
	TOGGLE(PORTD, PD5);
}

void	toggle_blue()
{
	TOGGLE(PORTD, PD3);
}

void	toggle_green()
{
	TOGGLE(PORTD, PD6);
}

void	set_red()
{
	SET(PORTD, PD5);
}

void	reset_red()
{
	RESET(PORTD, PD5);
}

void	set_green()
{
	SET(PORTD, PD6);
}

void	reset_green()
{
	RESET(PORTD, PD6);
}

void	set_blue()
{
	SET(PORTD, PD3);
}

void	reset_blue()
{
	RESET(PORTD, PD3);
}

void	rgb_reset_all()
{
	RESET(PORTD, PD3);
	RESET(PORTD, PD5);
	RESET(PORTD, PD6);
}