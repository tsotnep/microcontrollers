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

#define LED1						BIT1
#define ZERO						0x00
#define ENABLE_MOTOR_BIT0			BIT0
#define DIRECTION_MOTOR_BIT1		BIT1
#define STEP_MOTOR_BIT2				BIT2
#define degree90					850
unsigned int count = 0;

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

	P2SEL |= STEP_MOTOR_BIT2;
	P2DIR |= (STEP_MOTOR_BIT2 + DIRECTION_MOTOR_BIT1 + ENABLE_MOTOR_BIT0); //SET THEM AS OUTPUTS

	/* CONFIGURATION OF TIMERS */
	TA1CCTL1 = OUTMOD_7;
	TA1CCR0 = 1000;
	TA1CCR1 = 1000;
	TA1CTL = TASSEL_2 + ID_2 + MC_1 + TAIE;
	/* CONFIGURATION OF TIMERS */

	P2OUT &= ~ENABLE_MOTOR_BIT0;		//.......0 ENABLE MOTOR
//	P2OUT |= ENABLE_MOTOR_BIT0;			//.......1 DISABLE MOTOR
//	P2OUT &= ~DIRECTION_MOTOR_BIT1;		//......0. DIRECTION -CLOCKWISE
	P2OUT |= DIRECTION_MOTOR_BIT1;		//......1. DIRECTION -COUNTER-CLOCKWISE

	__enable_interrupt(); //simply set Global Iinterrupt Enable bit to 1

	count = 2 * degree90;
	while (1) {
		if ((int) count < 0)
			P2OUT &= ~DIRECTION_MOTOR_BIT1;		//rotates motor clockwise
		if ((int) count < -degree90)
			P2OUT |= ENABLE_MOTOR_BIT0; //disables the motor
	}
	return 0;
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A(void) {
	switch (TA1IV) {
	case TA1IV_TAIFG:			// Overflow
		count--;
		break;
	}
}
