/***************************************************************************************************
 *	Author: Tsotne Putkaradze, tsotnep@gmail.com
 *	Professor: Thomas Hollstein
 *	Lab Assitant: Siavoosh Payandeh Azad, Mairo Leier
 *	University: Tallinn Technical University, subject: IAY0330
 *	Board: TI - MSP430g2553
 *	Manual of Board: http://www.ti.com/lit/ug/slau144j/slau144j.pdf
 *	Datasheet of Board: http://www.ti.com/lit/ds/symlink/msp430g2553.pdf
 *	Description of Software:
 *
 ***************************************************************************************************/

#include <msp430g2553.h>

#define LED1	BIT0

//unsigned char ExampleFunction(unsigned int ticks);

int main(void) {

	/* DO NOT MODIFY */
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	// Configure CPU clock+
	if (CALBC1_1MHZ == 0xFF) {		    // If calibration constant erased
		while (1)
			;                   // do not load, trap CPU
	}
	DCOCTL = 1;                     // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;          // Set DCO to 1MHz //that was _1MHZ
	DCOCTL = CALDCO_1MHZ;
	/* END OF DO NOT MODIFY */

	/* CONFIGURATION OF BOARD */
	TA0CCTL0 = CCIE; //enable interrupt of Timer 0
	TA0CCR0 = 166; //number to which it counts up to
	TA0CTL = TASSEL_1 + ID_3 + MC_1; //timer0: 1-which_clock; 2-divede_on_2^3; 3-configure to count up to number stored in TA0CCR0
	__enable_interrupt(); //simply set Global Iinterrupt Enable bit to 1, same as: __bis_SR_register(GIE);
	//__bis_SR_register(GIE); //enables interrupt, if you set to 0, this bit inside interrupt means nested interrupt and priorities no longer exist, source: 2.2.3.3 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
	//__bis_SR_register(LPM0); //Low Power Mode 0: Disabled: CPU, MCLK; Enabled: ACLK, SMCLK
	/* CONFIGURATION OF BOARD */

	/* LOGIC */
	P1DIR |= 0x01; 				//configure Pin1.0 as output

	return 0;

	/* LOGIC */
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
	P1OUT ^= 0x01;
}

unsigned char ExampleFunction(unsigned int x) {
	unsigned int i = 0;
	P1OUT ^= 0x01; //toggles output P1.1.
	while (x--) {
		for (i = 0; i < 20000; i++)
			//waits or does something
			;
	}
	return 0;
}
