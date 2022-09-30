// Include the Device header
#include "tm4c123gh6pm.h"
#include<stdint.h>  //Require to use uint8_t

#define PC 0x04
#define PE 0x10
#define PB 0x02
#define RS 1	// 0x01, PB0
#define RW 2	// 0x02, PB1
#define EN 4	// 0x04, PB2

// The followng functions, KEYPAD_Init and getKey, are to be called by your new main funciton.
void LCD_4Bits_Init(void);	//Device Driver
void LCD_Write4Bits(unsigned char data, unsigned char control);
void LCD4Bits_Cmd(unsigned char command); //Device Driver
void LCD4Bits_Data(unsigned char data);		//Device Driver
void delay_micro(int n);
void delay_milli(int n);

void KEYPAD_Init(void){
		// enable PC clock;
		// enable PE clock;
		SYSCTL_RCGCGPIO_R |= PC | PE;
		// outputs on PE0 - 3
		GPIO_PORTE_DEN_R =0x0F;
		// digital I/O on PE0 - 3
		GPIO_PORTE_DIR_R =0x0F;
		// open drain on PE0 - 3
		GPIO_PORTE_ODR_R =0x0F;
		// PC4 to PC7 are inputs.
		GPIO_PORTC_DIR_R =0x00;
		// PC4 to PC7 are digital I/O.
		GPIO_PORTC_DEN_R =0xF0;
		// PC4 to PC7 configured with internal pull-up resistors and power.
		GPIO_PORTC_PUR_R =0xF0;
}

unsigned char getKey(void){
	const unsigned char keymap[4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
	};
	
	uint8_t k_row, k_col;
		// set all rows to GND by PE GPIODATA
		GPIO_PORTE_DATA_R = 0x00;   
		// read the key inputs on PC4-7 by PC GPIODATA.
		k_col = (GPIO_PORTC_DATA_R & 0xF0); 	//b7 b6 b5 b4 b3 b2 b1 b0
																//1  1  1  1  0  0  0  0  
	if(k_col == 0xF0){
		return 0;	// no key is pressed
	}
	
	for(;;){			// purpose: this forever loop determines which row detects a key stroke.
		k_row = 0;
			// 0000 1110, PE0=0, to GND
		GPIO_PORTE_DATA_R = 0x0E;
		delay_micro(10);	// debounce mechanical key
		k_col = (GPIO_PORTC_DATA_R & 0xF0) ;	// read the key inputs on PC4-7.
		if(k_col != 0xF0 )	// if there is a key pressed.
			break;
		
		k_row = 1;
			// 0000 1101, PE1=0, to GND
		GPIO_PORTE_DATA_R = 0x0D;
		delay_micro(10);	// debounce mechanical key
		k_col = (GPIO_PORTC_DATA_R & 0xF0) ;	// read the key inputs on PC4-7.
		if(k_col != 0xF0 )	// if there is a key pressed.
			break;
		
		k_row = 2;
			// 0000 1011, PE2=0, to GND
		GPIO_PORTE_DATA_R = 0x0B;
		delay_micro(10);	// debounce mechanical key
		k_col = (GPIO_PORTC_DATA_R & 0xF0) ;	// read the key inputs on PC4-7.
		if(k_col != 0xF0)	// if there is a key pressed.
			break;
		
		k_row = 3;
			// 0000 0111, PE2=0, to GND
		GPIO_PORTE_DATA_R = 0x07;
		delay_micro(10);	// debounce mechanical key
		k_col = (GPIO_PORTC_DATA_R & 0xF0) ;	// read the key inputs on PC4-7.
		if(k_col != 0xF0 )	// if there is a key pressed.
			break;
	
		return 0;
	}
	
	if(k_col == 0xE0){			// detect if the column on PC4 has a key presseed: 1110 0000 = 14
		return keymap[k_row][0];	// return thekey at 'k_row' and the column on PC4
	}
	
	if(k_col == 0xD0  ){			// detect if the column on PC5 has a key presseed: 1101 0000 = 13
		return keymap[k_row][1];	// return thekey at 'k_row' and the column on PC5
	}
	
	if(k_col == 0xB0 ){			// detect if the column on PC6 has a key presseed: 1011 0000 = 11
		return keymap[k_row][2];	// return thekey at 'k_row' and the column on PC6
	}
	
	if(k_col == 0x70 ){			// detect if the column on PC7 has a key presseed: 0111 0000 = 7
		return keymap[k_row][3];	// return thekey at 'k_row' and the column on PC7
	}
	
	return 0;
	
}

// Suppose you have finished theLCD 4-bitcode and the above Keypad functions.
// Then update your main function to capture key strokes and display characters on the LCD.

int main(){
	unsigned char key;
	int key_count = 0;
		// Initialize the keypad
	KEYPAD_Init();
		// Initialize the LCD.
	LCD_4Bits_Init();
	LCD4Bits_Cmd(0x01);
	LCD4Bits_Cmd(0x80);
	delay_milli(500);
	LCD4Bits_Data('A');
	while(1){
		key = getKey() ;	// call the function to get a key input
		delay_milli(800);	// give the mechanic key some time to debounce.
		if(key != 0){
			key_count++;
			if(key_count >= 16){
				key_count = 0;	// It's better to clear the LCD screen too.
			}
				// Display the key input on your LCD.
			LCD4Bits_Data(key);
			delay_milli(500);
		}
	}
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

void LCD_4Bits_Init(void){  //Can initialize both PA & PB!
		// Activate clk to PB							b7 b6 b5 b4 b3 b2 b1 b0
				SYSCTL_RCGCGPIO_R |= PB; //		
		// Define the directions on PB pins
				GPIO_PORTB_DIR_R |= 0xff; // output: 1 & input: 0
		// Enable the pins of PB
				GPIO_PORTB_DEN_R |=0xFF ; // I/o enabled: 1 & I/o NOT enabled: 0
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
