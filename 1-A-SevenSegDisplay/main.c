/**
 * main.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *  Updated on: 24 Feb 2024
 *      Author: David Cowell
 */

/**************************************************************************
 * DO NOT MODIFY THIS FILE
 * YOU MUST COMPLETE THE ASSIGNMENT WITHOUT MAKING ANY CHANGES TO THIS FILE
 **************************************************************************/

// Include drivers.
#include "DE1SoC_SevenSeg/DE1SoC_SevenSeg.h"
#include "HPS_Watchdog/HPS_Watchdog.h"

// Select which displays to use.
#define SINGLE_DISPLAY_LOCATION 0
#define DOUBLE_HEX_DISPLAY_LOCATION 2
#define DOUBLE_DEC_DISPLAY_LOCATION 4

// Peripheral base addresses.
volatile unsigned int *key_ptr = (unsigned int *)0xFF200050;

// Store the state of the keys last time we checked.
// This allows us to determine when a key is pressed, then released.
unsigned int key_last_state = 0;

/**
 * getPressedKeys
 *
 * Helper function to determine which keys have been pressed
 * and then released later.
 *
 * Arguments:
 * 		key_last_state: 	4-value array of the last state of each key.
 *
 * Returns: Mask, 1 for keys which have been pressed, 0 for keys not.
 */
unsigned int getPressedKeys() {

    // Store the current state of the keys.
    unsigned int key_current_state = *key_ptr;

    // If the key was down last cycle, and is up now, mark as pressed.
    unsigned int keys_pressed = (~key_current_state) & (key_last_state);

    // Save the key state for next time, so we can compare the next state to this.
    key_last_state = key_current_state;

    // Return result.
    return keys_pressed;
}

/**
 * main
 *
 * The entry function for this assignment. This function increments the
 * value of seven segment 0 (right-most display) when the user presses
 * Button 0 (right-most button), and increases the value of the pair of
 * seven segments 4 and 5 (left-most display pair) when the user presses
 * Button 3 (left-most button).
 *
 * This function is not supposed to be changed. It is simply control
 * logic that will allow you to test and verify your driver design.
 */
int main(void) {

    // Corresponding value set to 1 when a key is pressed then released.
    unsigned int keys_pressed = 0;

    // Initial value of each display output.
    unsigned int single_hex_display_value = 0;
    unsigned int double_hex_display_value = 0;
    unsigned int double_dec_display_value = 0;

    // Set displays to have initial values.
    DE1SoC_SevenSeg_SetSingle(SINGLE_DISPLAY_LOCATION, single_hex_display_value);
    DE1SoC_SevenSeg_SetDoubleHex(DOUBLE_HEX_DISPLAY_LOCATION, double_hex_display_value);
    DE1SoC_SevenSeg_SetDoubleDec(DOUBLE_DEC_DISPLAY_LOCATION, double_dec_display_value);

    while (1) {

        // Check if any buttons have been pressed and released.
        // The keys_pressed variable will need to be bit masked to determine if a
        // specific key was pressed.
        keys_pressed = getPressedKeys();

        // Test if each key has been pressed in turn, carry out action if so.
        if (keys_pressed & 0x1) {

            // If KEY0 was pressed, increment the single hex display value, update
            // display.
            single_hex_display_value++;
            DE1SoC_SevenSeg_SetSingle(SINGLE_DISPLAY_LOCATION,
                                      single_hex_display_value);

        } else if (keys_pressed & 0x2) {

            // If KEY1 was pressed, increment the double hex display value, update
            // display.
            double_hex_display_value++;
            DE1SoC_SevenSeg_SetDoubleHex(DOUBLE_HEX_DISPLAY_LOCATION,
                                         double_hex_display_value);

        } else if (keys_pressed & 0x4) {

            // If KEY2 was pressed, increment the double hex display value, update
            // display.
            double_dec_display_value++;
            DE1SoC_SevenSeg_SetDoubleDec(DOUBLE_DEC_DISPLAY_LOCATION,
                                         double_dec_display_value);

        } else if (keys_pressed & 0x8) {

            // If KEY3 was pressed, reset all displays to zero.
            single_hex_display_value = 0;
            double_hex_display_value = 0;
            double_dec_display_value = 0;
            DE1SoC_SevenSeg_SetSingle(SINGLE_DISPLAY_LOCATION,
                                      single_hex_display_value);
            DE1SoC_SevenSeg_SetDoubleHex(DOUBLE_HEX_DISPLAY_LOCATION,
                                         double_hex_display_value);
            DE1SoC_SevenSeg_SetDoubleDec(DOUBLE_DEC_DISPLAY_LOCATION,
                                         double_dec_display_value);
        }

        // Reset the watchdog timer so the DE1SoC does not reset.
        HPS_ResetWatchdog();
    }
}
