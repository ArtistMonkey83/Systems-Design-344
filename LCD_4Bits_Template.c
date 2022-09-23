// Include the Device header
#include "tm4c123gh6pm.h"

// You may define user-friendly macros here, as needed.
#define RS 1	// 0x01, PB0
#define RW 2	// 0x02, PB1
#define EN 4	// 0x04, PB2
#define PB 0x02

// These are thefunction signatures of the "device drivers".
// You will need to implement them in detail below, after the main() function.
// LCD4Bits_Cmd() and LCD4Bits_Data() would call LCD_Write4Bits().
void LCD_4Bits_Init(void);	//Device Driver
void LCD_Write4Bits(unsigned char data, unsigned char control);
void LCD4Bits_Cmd(unsigned char command); //Device Driver
void LCD4Bits_Data(unsigned char data);		//Device Driver
void delay_micro(int n);
void delay_milli(int n);

int main(){
	// call the initialization function
		LCD_4Bits_Init(); //Initializes both PB and PA if doing 8 bits!
	// Clear the LCD screen
		LCD4Bits_Cmd(0x1);		//0x01 clears the display
	// Set the cursor to the beginning of the first line
		LCD4Bits_Cmd(0x80); // PB0 == Register Select 0: Command Register 1: Data Registers (text)
	// Allow some delay, so the slow LCD will catch up with the fast MCU.
	delay_milli(500);
	// Test the LCD by sending a character.		b7 b6 b5 b4 b3 b2 b1 b0
			LCD4Bits_Data('1');		//        1  0  0  1  0  1  1  1  == 0x97  == "A"
																	//					LCD Data  | LCD Control	
	// Allow some delay, so the slow LCD will catch up with the fast MCU.
	delay_milli(500);
		// Test the LCD by sending a character.		b7 b6 b5 b4 b3 b2 b1 b0
			LCD4Bits_Data('a');		//          1  0  0  1  0  1  1  1  == 0x97  == "A"
																	//					LCD Data  | LCD Control
	// Allow some delay, so the slow LCD will catch up with the fast MCU.
	delay_milli(500);
}


void LCD_4Bits_Init(void){  //Can initialize both PA & PB!
		// Activate clk to PB							b7 b6 b5 b4 b3 b2 b1 b0
				SYSCTL_RCGCGPIO_R = PB; //		
		// Define the directions on PB pins
				GPIO_PORTB_DIR_R = 0xff; // output: 1 & input: 0
		// Enable the pins of PB
				GPIO_PORTB_DEN_R =0xFF ; // I/o enabled: 1 & I/o NOT enabled: 0
		// 4-bit data, 2 lines, 5*7 fonts
				LCD4Bits_Cmd(0x28); 		// b7 b6 b5 b4 b3 b2 b1 b0
																// 0  0  1  1  1  0  0  0 ==
		// increment the cursor automatically
				LCD4Bits_Cmd(0x06); 		// b7 b6 b5 b4 b3 b2 b1 b0
																// 0  0  0  0  0  1  1  0 ==
		// turn on the display
				LCD4Bits_Cmd(0x0f);			// b7 b6 b5 b4 b3 b2 b1 b0
																// 0  0  0  0  1  1  1  1 ==
		// clear the display
				LCD4Bits_Cmd(0x01);			// b7 b6 b5 b4 b3 b2 b1 b0
																// 0  0  0  0  0  0  0  1 ==	
}

void LCD_Write4Bits(unsigned char data, unsigned char control){
	// Remember, each time the 4-bit mode only allows sending 4-bit data. Extract the upper 4-bit data.
			unsigned char cmd_upper = data & 0xf0;		// b7 b6 b5 b4 b3 b2 b1 b0
																								// 1  1  1  1  0  0  0  0 == 0xf0 (anding with upper 4-bits)
	
	// Extract the lower 4-bit data. The lower 4-bit are control signals to thecontrol pins of the LCD.
			control &= 0x0f;		// b7 b6 b5 b4 b3 b2 b1 b0
																								// 0  0  0  0  1  1  1  1 == 0x0f (anding with lower 4-bits)
			//cmd_lower << 4; //Bit shift by 4 because first 4 bits are ALWAYS CONTROL // b7 b6 b5 b4 b3 b2 b1 b0
																																							 // 0  0  0  0  0  0  0  1 ==
	
	// send both data and the control signals  to LCD through the GPIODATA register.
			GPIO_PORTB_DATA_R = cmd_upper| control;

	// send both data and the control signals  to LCD through the GPIODATA register. Secure the sent signals by EN.
			GPIO_PORTB_DATA_R = cmd_upper| control|0x04; // b7 b6 b5 b4 b3 b2 b1 b0
																// 1  1  1  1  0  0  0  1 ==
																// RS 0, rW 0, E 1
	delay_micro(3);		// Allow some delay
	// Set back to the data without EN. 
			GPIO_PORTB_DATA_R = 0x00; 
	
}

void LCD4Bits_Cmd(unsigned char command){ //set command
	// Write the upper 4 bits. the control bit RS is 0 because this is a command not an actual text data.
		LCD_Write4Bits(command & 0xf0,0); 

	// Write the lower 4 bits of the command.
		LCD_Write4Bits(command << 4, 0);
	
	if(command<4)
		delay_milli(2);
	else
		delay_micro(37);
}

void LCD4Bits_Data(unsigned char data){ //set data
	// the control bit RS is 1 because this is to select the data register.
		LCD_Write4Bits(data & 0xf0,0x01);

	// the lower 4 bits of the text data.
	LCD_Write4Bits(data << 4, 0x01);
	
	
	delay_micro(37);
}


void delay_micro(int n){
	int i, j;
	for(i=0; i<n; i++){
		for(j=0;j<3;j++){
		}
	}
}

void delay_milli(int n){
	int i, j;
	for(i=0; i<n; i++){
		for(j=0;j<3180;j++){
		}
	}
}