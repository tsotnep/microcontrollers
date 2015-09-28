uthor: Tsotne Putkaradze, tsotnep@gmail.com

* Professor: Thomas Hollstein

* Lab Assitant: Siavoosh Payandeh Azad, Mairo Leier

* University: Tallinn Technical University, subject: IAY0330

* Board: TI - MSP430g2553

* Manual of Board: http://www.ti.com/lit/ug/slau144j/slau144j.pdf

* Datasheet of Board: http://www.ti.com/lit/ds/symlink/msp430g2553.pdf

* Description of Software:

*	simply rotating motor, endlessly

***************************************************************************************************/



#include <msp430g2553.h>



#define LED1 0x01

#define ZERO 0x00

#define ENABLE_MOTOR_BIT0 BIT0

#define DIRECTION_MOTOR_BIT1 BIT1

#define STEP_MOTOR_BIT2 BIT2



//unsigned char ExampleFunction(unsigned int ticks);



int main(void) {



/* DO NOT MODIFY */

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

// Configure CPU clock+

if (CALBC1_1MHZ == 0xFF) { // If calibration constant erased

while (1)

; // do not load, trap CPU

}

DCOCTL = 1; // Select lowest DCOx and MODx settings

BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz //that was _1MHZ

DCOCTL = CALDCO_1MHZ;

/* END OF DO NOT MODIFY */



/* CONFIGURATION OF TIMERS */

TA1CCTL0 = CCIE; //enable interrupt of Timer 0

TA1CCR0 = 50; //number to which it counts up to

TA1CCR1 = 100;

TA1CTL = TASSEL_1 + ID_0 + MC_1; //timer0: 1-which_clock; 2-divede_on_2^3; 3-configure to count up to number stored in TA0CCR0

TA1CCTL1 = OUTMOD_7;

/* CONFIGURATION OF TIMERS */



__enable_interrupt(); //simply set Global Iinterrupt Enable bit to 1, same as: __bis_SR_register(GIE);



P2DIR |= (STEP_MOTOR_BIT2 + DIRECTION_MOTOR_BIT1 + ENABLE_MOTOR_BIT0); //SET THEM AS OUTPUTS



P2OUT &= ~ENABLE_MOTOR_BIT0; //.......0 ENABLE MOTOR

// P2OUT |= ENABLE_MOTOR_BIT0; //.......1 DISABLE MOTOR



P2OUT &= ~DIRECTION_MOTOR_BIT1; //......0. DIRECTION -CLOCKWISE

// P2OUT |= DIRECTION_MOTOR_BIT1; //......1. DIRECTION -COUNTER-CLOCKWISE



while (1){





}



return 0;

}



#pragma vector=TIMER1_A0_VECTOR

__interrupt void Timer_A(void) {

P2OUT ^= STEP_MOTOR_BIT2; //TOGGLE STEP OF MOTOR

}
