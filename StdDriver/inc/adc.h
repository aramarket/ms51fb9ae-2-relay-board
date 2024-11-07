#ifndef ADC_H
#define ADC_H

// Define pin constants with underscores
#define P_17 0  // Corresponds to ADC Channel 0
#define P_30 1  // Corresponds to ADC Channel 1
#define P_07 2  // Corresponds to ADC Channel 2
#define P_06 3  // Corresponds to ADC Channel 3
#define P_05 4  // Corresponds to ADC Channel 4
#define P_04 5  // Corresponds to ADC Channel 5
#define P_03 6  // Corresponds to ADC Channel 6
#define P_11 7  // Corresponds to ADC Channel 7


// Function prototypes
void int_ADC();              // Function to initialize ADC
unsigned int read_ADC(int channel);     // Function to read ADC value
unsigned int calculate_ac_voltage(int channel);

#endif // ADC_H
 