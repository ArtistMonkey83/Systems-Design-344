#include "tm4c123gh6pm.h"
#define SW1_PRESSED 0x00    // Negative logic,the Switch is pressed.
#include<stdint.h>
void delayMs(int n);

int main(void)
	
{  
			unsigned char digitPattern[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
			//uint32_t sw_state;  
      /*the hex number for 7-seg display */
			#define PA 0x01  //RCGCGPIO for controlling Port A
			#define PB 0x02  //RCGCGPIO for controlling Port B
			
      /* enable clock to GPIOA */
			SYSCTL_RCGCGPIO_R = PA;        //Activate Port A
				
      /* enable clock to GPIOB */
			SYSCTL_RCGCGPIO_R = PB;        //Activate Port B
			
      /* set all PORTB pins as output to drive segments */
			GPIO_PORTB_DIR_R = 0xFF; //8  4  2  1   8  4  2  1
															 //1  1  1  1   1  1  1  1 == 0xFF	
															 //b7 b6 b5 b4  b3 b2 b1 b0
    
      /* set all PORTB pins as digital pins */
			GPIO_PORTB_DEN_R = 0XFF; //enabled is 1, not enabled is 0
	
      /* set PORTA pin 7-6 as output to select digit */
			GPIO_PORTA_DIR_R = 0xC0; //8  4  2  1   8  4  2  1
															 //1  1  0  0   0  0  0  0 == 0xC0	
															 //b7 b6 b5 b4  b3 b2 b1 b0
															 
      /* set PORTA pin 7-6 as digital pins */
			GPIO_PORTA_DEN_R = 0xC0;        //DEN for PA7 and PA6

		/* Task 1: Detecting push button. If pressed, turn on the LED. Else, turn off the LED.*/

		/* Task 2: Silmutaneously display two numbers on two 7-segments. */		
    for(;;)
    {
        /* drive pattern of the First number on the segments */
				GPIO_PORTB_DATA_R |= digitPattern[0]; //Display '0'
        /* select left digit by PA7*/
			  GPIO_PORTA_DATA_R |= 0x1; //Send a High to pin PA7
        /* delay 5 ms will result in 10 ms per loop, making human eyes not detect on/off of thedisplay */
        delayMs(5);          
        /* drive pattern of the Second number on the segments */
				GPIO_PORTB_DATA_R |= digitPattern[1]; //Display '1'
        /* select right digit by PA6*/
				GPIO_PORTA_DATA_R |= 0x1;  //Send a High to pin PA6
        /* delay 5 ms will result in 10 ms per loop */     
				delayMs(5);
    }
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}
