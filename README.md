# Seven Segment Display Driver Assessment

This repository is where you will develop a driver for the Seven Segment Display peripherals on the DE1-SoC.

Please refer to the assignment specification on Minerva for more details.

The following files are provided:

| File | Purpose |
| ---  | --- |
| `1-A-SevenSegDisplay/main.c`        | A test program which will allow you to test your solution (**do not modify**). |
| `1-A-SevenSegDisplay/DE1SoC_SevenSeg/DE1SoC_SevenSeg.h` | The header file which defines the interface for the driver (**do not modify**). |
| `1-A-SevenSegDisplay/DE1SoC_SevenSeg/DE1SoC_SevenSeg.c` | The implementation file for the driver. You need to complete this file. |

Please note you will need to create and configure the project in Arm Development Studio.
  
## `DE1SoC_SevenSeg Driver README`

### Overview

This driver is designed for controlling seven-segment displays on the DE1-SoC FPGA board. It provides functions to write values to individual displays, set pairs for hexadecimal or decimal numbers, and includes base addresses for the seven-segment display peripherals.

### Contents

1. [Base Addresses](#base-addresses)
2. [Functions](#functions)
3. [Examples](#examples)

### Base Addresses

The driver defines base addresses for two sets of seven-segment displays:
- `sevenseg_base_lo_ptr`: Base address for HEX 3, HEX 2, HEX 1, and HEX 0 displays i.e. 0xFF200020.
- `sevenseg_base_hi_ptr`: Base address for HEX 5 and HEX 4 displays i.e. 0XFF200030.

### Functions

`void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value)`

Writes a value to the specified seven-segment display if the display is less than SEVENSEG_N_DISPLAYS_LO 4 then it sends value to dispalay to the HEX 0, HEX 1, HEX 2 and HEX 3 base address else it sends the value to display to the base address of the HEX 4 and HEX 5.

`void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value)`

Sets a single seven-segment display with the provided value. The function automatically maps the value to the corresponding bits for illumination if the value is less than equal to 0xF i.e if the key is pressed is less than 0xF the it mapps the seven segment Leds to the display the value by calling the function DE1SoC_SevenSeg_Write(display,segment_mapping[value]) and using segment_mapping[] array if it is grater than the 0xF then it diaplays dash by calling DE1SoC_SevenSeg_Write(display,0x40).

`void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value)`

Sets a pair of seven-segment displays to represent a hexadecimal number If the value grater than 0xff then it represents dash that it means it is out of order oe else It calls `DE1SoC_SevenSeg_SetSingle` twice for the lower and higher nibbles for the lower nibble we call by DE1SoC_SevenSeg_SetSingle(display, value & 0xF) here the lower 4 bits value is extracted by doing & operation with 0xF and for the higher nibble we call by DE1SoC_SevenSeg_SetSingle(display+1, (value>>4) & 0xF) here the value is right shifted by 4 bits because the last 4 bits will come to the first 4 bits neglaecting the first 4 bits and extracting by & operation with 0xF.

`void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value)`

Sets a pair of seven-segment displays to represent a decimal number If the value grater than 99 then it represents dash that it means it is out of order It decomposes the decimal value into its digits and calls `DE1SoC_SevenSeg_SetSingle` for each. by calling the function DE1SoC_SevenSeg_SetSingle(display, value % 10); to diplay units place in the first seven segment here it does modulo operation to extract remainder i.e. units place and to display the tens place in the other seven segment display by calling the function DE1SoC_SevenSeg_SetSingle(display+1, value/10); here it divides the value by 10 to display the tens place on the seven segment display.  

## Examples

### Example 1: Writing to a Single Display

DE1SoC_SevenSeg_Write(0, 0x05); // Writes '5' to the first seven-segment display.

### Example 2 : Displaying a pair for Hexadecimal Number 

DE1SoC_SevenSeg_SetDoubleHex(0, 0x1E); // Displays '1E' on the first and second seven-segment displays.

### Example 3 : Desplaying a pair for Decimal Number 

DE1SoC_SevenSeg_SetDoubleDec(0, 30); // Displays '30' on the first and second seven-segment displays.

# DE1SoC_SevenSeg Driver Usage main() function.

## Overview

This README provides details on the usage of the `DE1SoC_SevenSeg` driver for controlling seven-segment displays on the DE1-SoC FPGA board. The provided `main.c` file is a sample application that increments the values displayed on specific segments in response to button presses.

## Contents

1. [File Structure](#file-structure)
2. [Peripheral Addresses](#peripheral-addresses)
3. [Key Input Handling](#key-input-handling)
4. [Main Function Logic](#main-function-logic)

## File Structure

- `main.c`: The main application file, which uses the `DE1SoC_SevenSeg` driver and includes key input handling logic.

## Peripheral Addresses

- `key_ptr`: Base address for the keys peripheral (`0xFF200050`).

## Key Input Handling

The `getPressedKeys` function is a helper function to determine which keys have been pressed and then released later. It compares the current and last state of the keys, identifying pressed keys.

## Main Function Logic

The `main` function initializes display values, sets initial values for the seven-segment displays, and enters a loop to continuously check for button presses. It increments specific display values based on button presses and updates the corresponding seven-segment displays.

### Incrementing Single Hex Display

if (keys_pressed & 0x1) {
    single_hex_display_value++;
    DE1SoC_SevenSeg_SetSingle(SINGLE_DISPLAY_LOCATION, single_hex_display_value);
}
here if the key 0 that read the address from by doing & 0x1 operation is pressed and released then it increments HEX 0 to display 0 to F it if Key 0 is pressed more than F then HEX 0 shows dash.

Pressing KEY0 increments the single hex display value .

### Incrementing Double Hex Display

} else if (keys_pressed & 0x2) {
    double_hex_display_value++;
    DE1SoC_SevenSeg_SetDoubleHex(DOUBLE_HEX_DISPLAY_LOCATION, double_hex_display_value);
}
here if the key 1 that read the address from by doing & 0x2 operation is pressed and released then it increments HEX 3 and HEX 2 to dispaly 00 to FF. 

Pressing KEY1 increments the double hex display value.

### Incrementing Double Decimal Display

} else if (keys_pressed & 0x4) {
    double_dec_display_value++;
    DE1SoC_SevenSeg_SetDoubleDec(DOUBLE_DEC_DISPLAY_LOCATION,
 double_dec_display_value);
}
here if the key 2 that read the addres from by doing & 0x3 operation is pressed and released then it increments HEX 5 and HEX 4 todisplay 00 to 99.

Pressing KEY2 increments the decimal display value.

### Resetting All Displays

} else if (keys_pressed & 0x8) {
    single_hex_display_value = 0;
    double_hex_display_value = 0;
    double_dec_display_value = 0;
    DE1SoC_SevenSeg_SetSingle(SINGLE_DISPLAY_LOCATION, single_hex_display_value);
    DE1SoC_SevenSeg_SetDoubleHex(DOUBLE_HEX_DISPLAY_LOCATION, double_hex_display_value);
    DE1SoC_SevenSeg_SetDoubleDec(DOUBLE_DEC_DISPLAY_LOCATION, double_dec_display_value);
}

Pressing KEY3 resets all displays to zero.