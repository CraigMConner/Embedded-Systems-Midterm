#include <stdint.h>
#include "HD44780.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

// Define button 1 and 2
#define MY_BUTTON1 PD7 
#define MY_BUTTON2 PD6 

// Variables for the temp sensors (analog pins they correspond too), average temp and final temp
int TMPa = 1, TMPb = 3, TMPc = 5, temp = 0, avg = 0;

// Prototypes for methods
short check_button_press_and_release(int button);
int getAnalog(int pin);
void printMenu();
int convert(int button, int avg);
void ADC_Init();

// Retrieves the value from a given analog pin
int getAnalog(int pin){
	// Enables ADC
        ADCSRA |= (1 << ADEN);
	// Clears out previous reading
	ADMUX &= 0xF0;
	// Chooses which analog pin to read from
	ADMUX |= pin;
	// Starts the conversion
	ADCSRA |= (1 << ADSC);
	// Waits for the ADC to finish
	while(ADCSRA & (1 << ADSC));
	// Disable ADC to prevent interference
	ADCSRA |= (0 << ADEN);
	return ADCW; 
}

// Sets up ADC functionality
void ADC_Init(){	
	// First three shifts are the Prescaler bits used for setting the clock frequency to 128 Hz
	ADCSRA |=  (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	// Sets the reference voltage to 1100 mV which is standard for temp sensing
	ADMUX  |= (1 << REFS1) | (1 << REFS0);
}

// Prints the default menu at the start and between readings
void printMenu()
{
        LCD_ClearLine(0);
	LCD_ClearLine(1);
	LCD_GotoXY(0, 0);
        LCD_PrintString("Btn 1: Deg in C");
        LCD_GotoXY(0, 1);
        LCD_PrintString("Btn 2: Deg in F");
}

// Calls each TMP36 to read the temp in the room
int getAvg() {
	TMPa = getAnalog(TMPa);
	// Delays implemented so they are not all reading at the same time. That woould cause interference
        _delay_ms(1000);
        TMPb = getAnalog(TMPb);
        _delay_ms(1000);
        TMPc = getAnalog(TMPc);
        // Variables for the displayed count and which press the system is on
        int avg = (TMPa + TMPb + TMPc) / 3;
	return avg;
}

int main(void)
{
	// Setups the LCD and ADC
	LCD_Setup();
	ADC_Init();
	// Print the main menu
	printMenu();

	while(1)
	{ 		
		// If button 2 is pressed, the average temp is taking and returned in Fahrenheit
		if (check_button_press_and_release(MY_BUTTON2)) {			
			avg = getAvg();
			temp = convert(2, avg);
			LCD_ClearLine(0);
			LCD_ClearLine(1);
			LCD_GotoXY(0,0);
			LCD_PrintInteger(temp);
			LCD_GotoXY(0,4);
			LCD_PrintString("F");
			_delay_ms(5000);
			printMenu();
		}
			
		// If button 1 is pressed, the average temp is taken and returned in Celsius
		if (check_button_press_and_release(MY_BUTTON1)) {
                        avg = getAvg();
			temp = convert(1, avg);
			LCD_ClearLine(0);
                        LCD_ClearLine(1);  
			LCD_GotoXY(0,0); 
			LCD_PrintInteger(temp);
			LCD_GotoXY(0,4);
                        LCD_PrintString("C");
			_delay_ms(5000);
                        printMenu();	
		}
	 }  
		
	return 0;
}

// Helper method to check if the button has been pressed
// Borrowed from our LCD lab
short check_button_press_and_release(int button)
{
	int ret_val = 0;

	if ((PIND & (1 << button)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << button)) != 0)
		{
			/* wait for button to be released */
			while((PIND & (1 << button)) != 0)
				ret_val = 1;
		}
	}

	return ret_val;
}

// Pressing button 1 returns the temp in C, 2 returns F 
int convert(int button, int avg)
{
	// Using 1100 as the reference voltage the formula for temp in C is used 
	int temp = (1100 * avg) / 1024;
        temp = (temp - 500) / -10;
	
	if (button == 1) 
	{	
		return temp - 30;	 
	}        
	if (button == 2) 
	{
		// Standard C to F conversion
		temp = (temp * (9/5)) + 32;
                return temp - 10;
	}	               
        return 0;       
}
