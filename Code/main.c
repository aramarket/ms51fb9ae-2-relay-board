#include "MS51_16K.h"
#include "seven_segment.h"
#include "adc.h"

#define output_pin   P_06
#define input_pin    P_05

#define relay_1_step_up   P16 // step up
#define relay_2_step_down   P15 // step down

#define NUM_SAMPLES     8      	// Number of samples for averaging
#define STEP_UP_THRESHOLD 216    // Upper limit to deactivate step-up relay
#define STEP_DOWN_THRESHOLD 239  // Upper limit for step-down relay
#define BUFFER_VOLT 1 



void pin_int(){

		P00_PUSHPULL_MODE;
		P04_PUSHPULL_MODE;
		P13_PUSHPULL_MODE;
		P12_PUSHPULL_MODE;
		P11_PUSHPULL_MODE;
		P01_PUSHPULL_MODE;
		P14_PUSHPULL_MODE;
	
		//PushPull_Mode for digits
		P10_PUSHPULL_MODE;
		P02_PUSHPULL_MODE;
		P03_PUSHPULL_MODE;
		P07_PUSHPULL_MODE;
		P30_PUSHPULL_MODE;
		P17_PUSHPULL_MODE;
		
		//For Relay
		P16_PUSHPULL_MODE;  //Relay1
		P15_PUSHPULL_MODE;	//Relay2
}


void main (void) {
    int i;
    int output_voltage, input_voltage;
    long output_sum = 0, input_sum = 0;
		
		pin_int();
		int_ADC();
		relay_1_step_up = 1;
		relay_2_step_down = 0;
		while (1) {
			// Reset sums for new sampling
			output_sum = 0;
			input_sum = 0;
			
			// Take 10 samples
			for (i = 0; i < NUM_SAMPLES; i++) {
				output_sum += calculate_ac_voltage(output_pin);  // Read and sum output ADC
				display_double_number(output_voltage, input_voltage);
				input_sum += calculate_ac_voltage(input_pin);    // Read and sum input ADC
				display_double_number(output_voltage, input_voltage);
			}
				
			// Calculate average voltage values
			output_voltage = (output_sum / NUM_SAMPLES);
			input_voltage = input_sum / NUM_SAMPLES;
			
        // Control logic with hysteresis
        if(input_voltage < STEP_UP_THRESHOLD - BUFFER_VOLT) {  //STEP_UP_THRESHOLD = 216 - 1 it work on 214
            relay_1_step_up = 1;
            relay_2_step_down = 0;
        } else if(input_voltage > STEP_DOWN_THRESHOLD + BUFFER_VOLT) { //STEP_DOWN_THRESHOLD = 239 + 1 it work on 241
            relay_1_step_up = 0; 
            relay_2_step_down = 1;
					//STEP_UP_THRESHOLD_HIGH = 216 && STEP_DOWN_THRESHOLD_HIGH = 239 it work within 216 - 239
        } else if(input_voltage >= STEP_UP_THRESHOLD && input_voltage <= STEP_DOWN_THRESHOLD) {
            relay_1_step_up = 0;
            relay_2_step_down = 0;
        }
		}
}






