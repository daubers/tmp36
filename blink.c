//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>				

#define REDLED   BIT4 // Port 2.4
#define GRNLED   BIT3 // Port 1.3

#define TMP36 INCH_6 // analogue input on 1.6


unsigned int analogRead(unsigned int pin) {
	ADC10CTL0 = ADC10ON + ADC10SHT_2 + SREF_0;
	ADC10CTL1 = ADC10SSEL_0 + pin;

	if (pin==INCH_4){
		ADC10AE0 = 0x10;
	}
	else if(pin==INCH_5){
		ADC10AE0 = 0x20;
	}
	else if(pin==INCH_6){
			ADC10AE0 = 0x40;
		}
	ADC10CTL0 |= ENC + ADC10SC;
	while (1) {
			if (((ADC10CTL0 & ADC10IFG)==ADC10IFG)) {
				ADC10CTL0 &= ~(ADC10IFG +ENC);
			break;
		}
	}
	return ADC10MEM;
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P2DIR |= REDLED;				// set p2.4 to output direction
	P2DIR |= GRNLED;				// Set P2.3 to output direction
	P2OUT = 0x00;
	ADC10CTL0=SREF_0;				//set the reference voltage
	ADC10CTL1=ADC10SSEL_1;			// set the ADC to use ACLK
	ADC10CTL1=ADC10DIV_3;			// clock factor div 4

	volatile float adcvalue1;

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization
		adcvalue1 = (analogRead( TMP36 )*(3300.0/1023.0) - 500)/10.0; //Read the analog input from channel 6
		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR
		if (adcvalue1 < 23){
			P2OUT = GRNLED;
		}
		else {
			P2OUT = REDLED;
		}
		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
	
	return 0;
}
