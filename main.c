/**
*@file main.c
*@Author Dirk Dubois
*@version 1.0
*@date Feb 12, 2011
*
*@brief A C program that displays a voltage measured on RB7 using the onboard ADC
*and displays it to a standard LCD display on PORTA running in 4bit mode.
*The voltage at RB7 is divide by a voltage divider of (1/4) requiring a scaling of 4.
*The voltage across the PIC of +5V is used to operate the ADC
*For details on the hookup of the LCD see lcd.c for more information.
*/

/*Includes*/
#include <htc.h>
#include <stdlib.h>
#include "lcd.h"
#include "delay.h"

/*Defines*/
#define _XTAL_FREQ 4000000 /**<A define for the clock frequency*/

/*Function Prototypes*/
char *ftoa(float f, int *status);

/**
*@brief An initialization function that sets the Tristate registers and oscillator
*The oscillator is set to a value of four mega hertz, and RB7 is set as an input
*@retval None
*/
void init(void) 
{
	OSCCON = 	0b01100000;	//set  internal oscillator frequency to 4 MHz
	TRISA = 	0b00000000; //set PORTA as output for LCD
	TRISB = 	0b10000000; //set PORTB outputs except RB7 as input
	ANSEL = 0x00;
}	

/**
*@brief An initalization function for the ADC on port RB7
*@retval
*/
void adcInit(void)
{
	ANSEL = 	0b01000000; //Sets the analogue select ports to RB7
	ADCON0 = 	0b11110000;	//Sets ADCON0
	ADCON1 = 	0b10000000;	//Sets ADCON1
	ADON = 		1; 			//Turn on the ADC
}	

/**
*@brief A function that starts the adc sampling
*@retval None
*/
void adcGo(void)
{
	GODONE = 1;
	while(GODONE);
	return;
}	

/**
*@brief A display function that takes a float and displays it to the 2nd line of the LCD.
*@param[in] value The value you wish to print to the display
*@retval None
*@note This functions uses the lcd library
*/
void lcd_Display(float value)
{
	lcd_goto(0x40);					//Goto the 2nd line
	
	char *buffer;					//Create a buffer for ftoa function
	int status;
	
	buffer = ftoa(value, &status);	//Ftoa function that converts the float to a string
	lcd_puts(buffer);				//Displays the buffer
	return;
}

/**
*@brief The main function that initalizes the peripherals 
 and displays the value returned by the adc in volts.
*@retval None
*/ 
void main(void)
{
	//Intialization function calls
	init();							//Intialize the PIC
	lcd_init(); 					//Initialize the lcd
	
	lcd_write(0x0C); 				//Turn off cursor
	
	lcd_goto(0x00); 				//Goto begining of the screen
	lcd_puts("Output Voltage"); 	//Display header

	adcInit();						//ADC setup
	
	//Values for the Voltage Meter
	int i; 							/**<Width of the Average*/
	int low = 0; 					/**<Low end of the ADC value */
	int high = 0;					/**<High end of the ADC value*/
	float Vin = 0; 					/**<To Calculate Vin*/
	float Avg;						/**<To Calculate the average*/
		
	//For the duration of the program
	while(1)
	{
		Avg = 0; 					// Clear the average
		
		//Take 128 samples for the average
		for(i=0; i<128; i++)
		{
			adcGo();				//Start the ADC
		
			low = 	ADRESL;			//Retrieve the ADC values
			high = 	ADRESH;
		
			high = high << 8; 		//Combine the high and low values of the ADC
			Vin = high + low;
			
			Vin = (Vin / 1024) * 5;	//Scale the ADC Values w.r.t +5V
			
			Avg = Avg + Vin ;
		}	
		
		Avg = Avg / 128; 			//Divide the average by its width i
		Avg = Avg * 4; 				//Convert Vin due to voltage divider circuit

		//Displays the voltage on the 2nd line and truncates decimal places
		lcd_Display(Avg);
		lcd_goto(0x46);
		lcd_puts(" Volts");
		   
	}

}	