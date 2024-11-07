#include "MS51_16K.h"
#include "adc.h"

#define V_REF           5.0    		// ADC reference voltage in volts
#define ADC_MAX         4095    	// 12-bit ADC
#define R1              330000.0 	// Ohms
#define R2              1500.0   	// Ohms
#define FACTOR_RMS      0.7071  	// 1/sqrt(2)
#define NUM_SAMPLES     100      	// Number of samples for averaging


void int_ADC(void);
unsigned int read_ADC(int channel);
unsigned int calculate_ac_voltage(int channel);

// Function to initialize the ADC
void int_ADC(void) {
    ENABLE_ADC_BANDGAP;  // Enable the Bandgap reference for the ADC
    
    AINDIDS = 0x00;      // Clear AINDIDS registerr
    AINDIDS |= SET_BIT4; // Select P05 as ADC input channel
    AINDIDS |= SET_BIT3; // Select P06 as ADC output channel
}

// Function to read the ADC value
unsigned int read_ADC(int channel) {
    unsigned int val = 0;
	
    // Enable the specified ADC channel based on the pin constant
    switch (channel) {
        case P_17: ENABLE_ADC_AIN0; break; // Read from ADC Channel 0 (P_17)
        case P_30: ENABLE_ADC_AIN1; break; // Read from ADC Channel 1 (P_30)
        case P_07: ENABLE_ADC_AIN2; break; // Read from ADC Channel 2 (P_07)
        case P_06: ENABLE_ADC_AIN3; break; // Read from ADC Channel 3 (P_06)
        case P_05: ENABLE_ADC_AIN4; break; // Read from ADC Channel 4 (P_05)
        case P_04: ENABLE_ADC_AIN5; break; // Read from ADC Channel 5 (P_04)
        case P_03: ENABLE_ADC_AIN6; break; // Read from ADC Channel 6 (P_03)
        case P_11: ENABLE_ADC_AIN7; break; // Read from ADC Channel 7 (P_11)
        default: return 0; // Invalid pin
    }
	
    clr_ADCF;              // Clear the ADC flag
    set_ADCS;              // Start ADC conversion

    while (ADCF == 0);     // Wait until the conversion is complete

    // Read the low byte of the ADC result
    val |= ADCRL;          // Get lower 8 bits of the result
    // Read the high byte of the ADC result
    val |= (ADCRH << 4);   // Get higher 4 bits and combine with low byte

    val &= 0xFFFF;         // Ensure value is 16 bits

    return val;            // Return the ADC value
}

// Function to convert ADC value to voltage with averaging
unsigned int calculate_ac_voltage(int channel) {
		int i;
		unsigned long adc_sum = 0;
    float V_adc, V_dc, V_rms, adc_avg;			

    // Take multiple samples and accumulate them
    for (i = 0; i < NUM_SAMPLES; i++) {
        adc_sum += read_ADC(channel);  // Read ADC value and add to the sum
    }
    
    // Calculate the average ADC value
    adc_avg = (float)adc_sum / NUM_SAMPLES;   //260 ADC value for 100vac

		// Convert ADC average to voltage
    V_adc = (adc_avg / ADC_MAX) * V_REF;  //0.317  Vdc voltage at pin of mc
                                                      
    // Calculate peak voltage before voltage divider
    V_dc = V_adc / (R2 / (R1 + R2));   //70.0    Vdc after voltage divider

    // Convert Vdc to Vac
    V_rms = V_dc / FACTOR_RMS;         // 99.07 Vac after convert dc to ac
		
		// Add Doide drop
		V_rms +=  0.9;                  // 100 Vac 
		
		// Constants for calibration
		V_rms *=  1.58;
		return (unsigned int)V_rms;
}