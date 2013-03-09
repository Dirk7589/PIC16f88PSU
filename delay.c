/**
*@file delay.c
*@author Dirk Dubois
*@version 1.0
*@date Feb 12, 2011
*@brief A set of delay functions for the pic16f88
*/

/*Includes*/
#include "delay.h"

/**
*@brief Delay specified number of milliseconds
*@param[in] cnt The amount of time you wish to delay in milliseconds
*@retval None
*/
void DelayMs(unsigned char cnt)
{
#if	XTAL_FREQ <= 2MHZ
	do {
		DelayUs(996);
	} while(--cnt);
#endif

#if    XTAL_FREQ > 2MHZ	
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
		} while(--i);
	} while(--cnt);
#endif
}
