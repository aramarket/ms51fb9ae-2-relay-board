#include "MS51_16K.h"

// Common Cathode

// Define pins for each segment
sbit SEG_A =  P0^0;
sbit SEG_B =  P0^4;
sbit SEG_C =  P1^3;
sbit SEG_D =  P1^2;
sbit SEG_E =  P1^1;
sbit SEG_F =  P0^1;
sbit SEG_G =  P1^4;
sbit SEG_DP = P2^0;

// Define pins for digit control for the first display
sbit DIGIT1_A = P1^0;   
sbit DIGIT2_A = P0^2;   
sbit DIGIT3_A = P0^3;   

// Define pins for digit control for the second display
sbit DIGIT1_B = P0^7;
sbit DIGIT2_B = P3^0;
sbit DIGIT3_B = P1^7;

// Lookup table for numbers 0-9 in common cathode 7-segment display format
const unsigned char segment_codes[10][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {1, 1, 1, 0, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};

// Delay function
void delay_ms(unsigned int ms) {
    while (ms--) {
        unsigned int i = 100; // controll brightness
        while (i--);
    }
}

// Set the segments for a specific digit
void set_segments(unsigned char value) {
    SEG_A = segment_codes[value][0];
    SEG_B = segment_codes[value][1];
    SEG_C = segment_codes[value][2];
    SEG_D = segment_codes[value][3];
    SEG_E = segment_codes[value][4];
    SEG_F = segment_codes[value][5];
    SEG_G = segment_codes[value][6];
    SEG_DP = segment_codes[value][7];
}

// Display a digit on one of the 3-digit displays
void display_digit(unsigned char display, unsigned char digit, unsigned char value) {
    // Turn off all digits first
    DIGIT1_A = 1; DIGIT2_A = 1; DIGIT3_A = 1;
    DIGIT1_B = 1; DIGIT2_B = 1; DIGIT3_B = 1;

    // Set the segments for the current digit
    set_segments(value);

    // Activate the current digit on the selected display
    if (display == 1) {
        if (digit == 1) DIGIT1_A = 0;
        if (digit == 2) DIGIT2_A = 0;
        if (digit == 3) DIGIT3_A = 0;
    } else if (display == 2) {
        if (digit == 1) DIGIT1_B = 0;
        if (digit == 2) DIGIT2_B = 0;
        if (digit == 3) DIGIT3_B = 0;
    }

    // Short delay for multiplexing
    delay_ms(1);
}

// Display a single 3-digit number on the specified display
void display_single_number(int number, unsigned char display) {
    // Break down number into hundreds, tens, and ones
    unsigned char hundreds = (number / 100) % 10;
    unsigned char tens = (number / 10) % 10;
    unsigned char ones = number % 10;

    // Display each digit for the specified display
    display_digit(display, 1, hundreds);
    display_digit(display, 2, tens);
    display_digit(display, 3, ones);
	
    // Turn off last digits
    DIGIT3_A = 1; DIGIT3_B = 1;
}

// Display both display 3-digit numbers (0-999 each)
void display_double_number(int number1, int number2) {
    // Break down numbers into hundreds, tens, and ones
    unsigned char hundreds1 = (number1 / 100) % 10;
    unsigned char tens1 = (number1 / 10) % 10;
    unsigned char ones1 = number1 % 10;

    unsigned char hundreds2 = (number2 / 100) % 10;
    unsigned char tens2 = (number2 / 10) % 10;
    unsigned char ones2 = number2 % 10;

    // Display each digit for both displays
    display_digit(1, 1, hundreds1);
    display_digit(1, 2, tens1);
    display_digit(1, 3, ones1);

    display_digit(2, 1, hundreds2);
    display_digit(2, 2, tens2);
    display_digit(2, 3, ones2);
		
    // Turn off last digits
    DIGIT3_A = 1; DIGIT3_B = 1;
}
