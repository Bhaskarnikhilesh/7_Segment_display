/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *  Updated on: 24 Feb 2024
 *      Author: David Cowell
 *  Updated on: 6  Mar 2024
 *      Author: Nikhilesh Bhaskar  
 */

#include "DE1SoC_SevenSeg.h"

// ToDo: Add the base addresses of the seven segment display peripherals.

//Added the base address for seven segment display for HEX 3, HEX 2, HEX 1 and HEX 0
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *)0xFF200020;
////Added the base address for seven segment display for HEX 5, HEX 4
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *)0XFF200030;

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
    // ToDo: Write the code for driving a single seven segment display here.
    // Your function should turn a real value 0-F into the correctly encoded
    // bits to enable the correct segments on the seven segment display to
    // illuminate. Use the DE1SoC_SevenSeg_Write function you created earlier
    // to set the bits of the display.

	// if the value is less than F to display then we will illuminate the values on seven segment display.
	if(value <= 0xF){
			//Mapping and assigning the values to their corresponding bit-mapping for seven- segment display
			//where each bits corresponds to to the led in seven segment we are enabling the bits
			//for corresponding values to display.
		static const unsigned char segment_mapping[] = {
						0x3F,//0 to represent it we enable the bits 0, 1, 2, 3, 4, 5
						0X06,//1 to represent it we enable the bits 1, 2
						0x5B,//2 to represent it we enable the bits 0, 1, 3, 4, 6
						0x4F,//3 to represent it we enable the bits 0, 1, 2, 3, 7
						0x66,//4 to represent it we enable the bits 1, 2, 5, 6
						0x6D,//5 to represent it we enable the bits 0, 2, 3, 5, 6
						0x7D,//6 to represent it we enable the bits 0, 2, 3, 4, 5, 6
						0x07,//7 to represent it we enable the bits 0, 1, 2
						0x7F,//8 to represent it we enable the bits 0, 1, 2, 3, 4, 5, 6 
						0x6F,//9 to represent it we enable the bits 0, 1, 2, 3, 5, 6
						0x77,//A to represent it we enable the bits 0, 1, 2, 4, 5, 6
						0x7C,//B to represent it we enable the bits 2, 3, 4, 5, 6
						0x39,//C to represent it we enable the bits 0, 3, 4, 5
						0x5E,//D to represent it we enable the bits 1, 2, 3, 4, 6
						0x79,//E to represent it we enable the bits 0, 3, 4, 5, 6
						0X71 //F to represent it we enable the bits 0, 4, 5, 6

				};
				DE1SoC_SevenSeg_Write(display,segment_mapping[value]);
			}else{
				DE1SoC_SevenSeg_Write(display, 0x40);
			}
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a HEXADECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *    input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     1E |        1 |      E
     *     0x60 |     60 |        6 |      0
     */
    if(value > 0xFF){
    // if the value is greater than ff then it displays dash for both seven segment displays.
		DE1SoC_SevenSeg_SetSingle(display,0x40);
		DE1SoC_SevenSeg_SetSingle(display+1,0x40);
	}
	else{
    // Here I am displaying hexadecimal on the seven segment Led pair
    //each seven segment should display as shown in the above examples
    //we doing this by calling DE1SoC_SevenSeg_SetSingle function to display each value
    //on the specified leds here the lower nibble is value is masked out by & 0xF to give only 
    //4 lower bits and again we will call the DE1SoC_SevenSeg_SetSingle function 
    //to display higher nibble in another Seven segment by incrementing the address by 1
    //and right shifting the value to 4 bits so that last 4 bits will be placed at the first 
    //4 bits and we can mask out by & 0xF so that only remaining value will be displayed on the next seven segment
		DE1SoC_SevenSeg_SetSingle(display,value & 0xF);
		DE1SoC_SevenSeg_SetSingle(display+1,(value >> 4) & 0xF);
	}
}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a DECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *	  input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     30 |        3 |      0
     *     0x60 |     96 |        9 |      6
     */
    if(value > 0x63 ){
    // if the value is greater than 99 then it displays dash for both seven segment displays.
		DE1SoC_SevenSeg_SetSingle(display,0x40);
		DE1SoC_SevenSeg_SetSingle(display+1,0x40);
	}
	else{
	// DE1SoC_SevenSeg_SetSingle(display, value % 10): 
    //This line sets the first seven-segment display (display) with the least significant digit of the value 
    //(obtained using the modulo operator %). 
    //This ensures that only the last digit of the decimal number is displayed on the specified seven-segment display.
	    DE1SoC_SevenSeg_SetSingle(display, value % 10);
    //DE1SoC_SevenSeg_SetSingle(display+1, value/10): 
    //This line sets the second seven-segment display (to the left of the first display) 
    //with the remaining digits of the value after removing the least significant digit (achieved by dividing value by 10). 
    //This effectively places the most significant digit of the decimal number on the second seven-segment display.
	DE1SoC_SevenSeg_SetSingle(display+1, value/10);
	}
}