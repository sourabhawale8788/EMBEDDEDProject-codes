/* 
	NAME : AWALE SOURABH SUNIL 				PRN : 22UET005
	NAME : BIRADAR PURVA BALASAHEB		PRN : 22UET009

	Lab Activity: Mini Project 
	Titles for Mini Project :
					Implement variable frequency generator using timer in LPC2148  
					
*/

#include <LPC214x.h>

#define FOSC            12000000UL          // 12 MHz external oscillator
#define PLL_M           5                   // (Multiplier+1); PLL_M=5: CCLK=60MHz
#define PLL_P           2                   // (Divider: 0=1, 1=2, 2=4, ...); PLL_P=2: P=4
#define CCLK            (FOSC * PLL_M)      // 60 MHz CPU clock
#define PCLK            (CCLK / 4)          // 15 MHz Peripheral clock (default VPBDIV=0)

#define ADC_PINSEL_MASK (3 << 24)           // For P0.28/AD0.1 in PINSEL1[25:24]
#define ADC_PINSEL_VAL  (1 << 24)

#define SQUARE_OUT_PIN  (1 << 21)           // P0.21

#define ADC_MAX_VAL     1023
#define MIN_FREQ        100                 // 100 Hz minimum
#define MAX_FREQ        10000               // 10 kHz maximum

void PLL_Config(void);
void ADC0_Init(void);
unsigned int ADC0_Read(void);
void GPIO_Init(void);
void Timer0_Delay(unsigned int cycles);
unsigned int Map_ADC_To_Freq(unsigned int adc_value);

int main() {
    unsigned int adc_value, freq, match_cycles;

    PLL_Config();
    GPIO_Init();
    ADC0_Init();

    while (1) {
        adc_value = ADC0_Read();
        freq = Map_ADC_To_Freq(adc_value);

        // Calculate match value for Timer delay (half period)
        match_cycles = PCLK / (2 * freq);

        // Toggle output pin for half period, repeat
        IO0SET = SQUARE_OUT_PIN;
        Timer0_Delay(match_cycles);
        IO0CLR = SQUARE_OUT_PIN;
        Timer0_Delay(match_cycles);
    }
}

void PLL_Config(void) {
    // PLL setup for 12MHz -> 60MHz (CCLK); PCLK = 15 MHz (default)
    PLL0CON = 0x01;                           // PLL enable
    PLL0CFG = ((PLL_M - 1) & 0x1F) | ((PLL_P << 5) & 0x60); // MSEL & PSEL
    PLL0FEED = 0xAA; 
		PLL0FEED = 0x55;         // Feed sequence

    while (!(PLL0STAT & (1 << 10)));          // Wait for PLL lock

    PLL0CON = 0x03;                           // PLL enable & connect
    PLL0FEED = 0xAA; PLL0FEED = 0x55;         // Feed update

    // VPB divider is default (0): PCLK = CCLK / 4
}

// ADC0 initialization (single channel, polling)
void ADC0_Init(void) {
    PINSEL1 &= ~ADC_PINSEL_MASK;              // Clear
    PINSEL1 |= ADC_PINSEL_VAL;                // P0.28 as AD0.1
    AD0CR = (1 << 1)          // SEL: select AD0.1
          | (4 << 8)          // CLKDIV: PCLK/5 = 3MHz ADC clock (below 4.5MHz limit)
          | (0 << 16)         // BURST = 0: Software controlled
          | (1 << 21);        // PDN = 1: Enable ADC
}

// Read single ADC conversion (polling)
unsigned int ADC0_Read(void) {
		unsigned int result;
    AD0CR |= (1 << 24);                        // Start conversion (START=1)
    while (!(AD0GDR & (1U << 31)));            // Wait for DONE bit
    result = (AD0GDR >> 6) & 0x3FF;
    return result;
}

// GPIO setup for output
void GPIO_Init(void) {
    IO0DIR |= SQUARE_OUT_PIN;                 // Set P0.21 as output
}

// Simple Timer0 delay using polling (for square wave timing)
void Timer0_Delay(unsigned int cycles) {
    T0TCR = 0x02;                             // Reset Timer0
    T0PR = 0;                                 // No prescaler
    T0MR0 = cycles;                           // Set match value
    T0MCR = 0x04;                             // Stop on MR0
    T0TCR = 0x01;                             // Enable counting

    while (T0TC < cycles);                    // Wait until match (TC reaches MR0)

    T0TCR = 0x00;                             // Stop timer
}

// Map ADC (0-1023) to output frequency (MIN_FREQ to MAX_FREQ)
unsigned int Map_ADC_To_Freq(unsigned int adc_value) {
    return MIN_FREQ + ((MAX_FREQ - MIN_FREQ) * adc_value) / ADC_MAX_VAL;
}
