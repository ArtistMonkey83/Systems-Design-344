// The followng functions, KEYPAD_Init and getKey, are to be called by your new main funciton.

void KEYPAD_Init(void){
		// enable PC clock;
		// enable PE clock;
	
		// outputs on PE0 - 3
		// digital I/O on PE0 - 3
		// open drain on PE0 - 3
	
		// PC4 to PC7 are inputs.
		// PC4 to PC7 are digital I/O.
		// PC4 to PC7 configured with internal pull-up resistors and power.
	
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
		// read the key inputs on PC4-7 by PC GPIODATA.
	
	if(k_col == 0xF0){
		return 0;	// no key is pressed
	}
	
	for(;;){			// purpose: this forever loop determines which row detects a key stroke.
		k_row = 0;
			// 0000 1110, PE0=0, to GND
		delay_micro(10);	// debounce mechanical key
		k_col = ;	// read the key inputs on PC4-7.
		if(k_col != )	// if there is a key pressed.
			break;
		
		k_row = 1;
			// 0000 1101, PE1=0, to GND
		delay_micro(10);	// debounce mechanical key
		k_col = ;	// read the key inputs on PC4-7.
		if(k_col != )	// if there is a key pressed.
			break;
		
		k_row = 2;
			// 0000 1011, PE2=0, to GND
		delay_micro(10);	// debounce mechanical key
		k_col = ;	// read the key inputs on PC4-7.
		if(k_col != )	// if there is a key pressed.
			break;
		
		k_row = 3;
			// 0000 0111, PE2=0, to GND
		delay_micro(10);	// debounce mechanical key
		k_col = ;	// read the key inputs on PC4-7.
		if(k_col != )	// if there is a key pressed.
			break;
	
		return 0;
	}
	
	if(k_col == ){			// detect if the column on PC4 has a key presseed: 1110 0000
		return keymap[][];	// return thekey at 'k_row' and the column on PC4
	}
	
	if(k_col == ){			// detect if the column on PC5 has a key presseed: 1101 0000
		return keymap[][];	// return thekey at 'k_row' and the column on PC5
	}
	
	if(k_col == ){			// detect if the column on PC6 has a key presseed: 1011 0000
		return keymap[][];	// return thekey at 'k_row' and the column on PC6
	}
	
	if(k_col == ){			// detect if the column on PC7 has a key presseed: 0111 0000
		return keymap[][];	// return thekey at 'k_row' and the column on PC7
	}
	
	return 0;
	
}

// Suppose you have finished theLCD 4-bitcode and the above Keypad functions.
// Then update your main function to capture key strokes and display characters on the LCD.

int main(){
	unsigned char key;
	int key_count = 0;
		// Initialize the keypad
		// Initialize the LCD.
	LCD4Bits_Cmd(0x01);
	LCD4Bits_Cmd(0x80);
	delay_milli(500);
	while(1){
		key = ;	// call the function to get a key input
		delay_milli(800);	// give the mechanic key some time to debounce.
		if(key != 0){
			key_count++;
			if(key_count >= 16){
				key_count = 0;	// It's better to clear the LCD screen too.
			}
				// Display the key input on your LCD.
			delay_milli(500);
		}
	}
}