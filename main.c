#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"

#define PA 0x01
#define PB 0x02

uint32_t delay = 0;
int i;
int main(){
		unsigned char digitPattern[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; // the array for the 7-segment display
    /*from Table 2-4 */
				// call the function to initialize PLL
		PLL_Init();
		
				// call the function to initialize SysTick
		SysTick_Init();
		  /* enable clock to GPIOA */
		  /* enable clock to GPIOB */
		SYSCTL_RCGCGPIO_R = PA | PB;
		 /* set all PORTB pins as output to drive segments */
		GPIO_PORTB_DIR_R = 0xFF;
     /* set all PORTB pins as digital pins */
		GPIO_PORTB_DEN_R =0xFF;
     /* set PORTA pin 7 as output to drive the DC motor */
		GPIO_PORTA_DIR_R = 0x80;
     /* set PORTA pin 7 as a digital pin */
		GPIO_PORTA_DEN_R = 0x80;
		// The code for the countdown timer. Hint: a for loop
		for(i = 9; i >= 0 ; i--){
			GPIO_PORTB_DATA_R = digitPattern[i];
			SysTick_Wait200ms(5);
		}                  
		// after the timed countdown, start the DC motor with the duty cycle to be 80%.
		while (1){
					// on for .8 ms, output through PA7
					GPIO_PORTA_DATA_R = 0x80;
					// assume 50 MHz clk, calculate the delay value for 0.8 ms 
				  // Use SysTick to create the 0.8 ms delay
					SysTick_Wait08ms(1);
					// off for .2 ms, output through PA7
					GPIO_PORTA_DATA_R = 0x00;
					// assume 50 MHz clk, calculate the delay value for 0.2 ms
					// Use SysTick to create the 0.2 ms delay
					SysTick_Wait02ms(1);
		}
		
}
