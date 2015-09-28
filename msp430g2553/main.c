/***************************************************************************************************
 *	Author: Tsotne Putkaradze, tsotnep@gmail.com
 *	Professor: Thomas Hollstein
 *	Lab Assitant: Siavoosh Payandeh Azad, Mairo Leier
 *	University: Tallinn Technical University, subject: IAY0330
 *	Board: TI - MSP430g2553
 *	Manual of Board: http://www.ti.com/lit/ug/slau144j/slau144j.pdf
 *	Datasheet of Board: http://www.ti.com/lit/ds/symlink/msp430g2553.pdf
 *	Description of Software:
 *		playing with interrupts, timers, uart
 *
 ***************************************************************************************************/

#include <msp430g2553.h>

#define LED1	BIT0
#define LED2	BIT6
#define BTN2	BIT3
//#define INCH_x	1010

//unsigned char ExampleFunction(unsigned int ticks);

int config(void) {

	/* WATCHDOG */
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	/* WATCHDOG */

	/* DO NOT MODIFY */
	// Configure CPU clock+
	if (CALBC1_1MHZ == 0xFF) { 		// If calibration constant erased
		while (1)
			// do not load, trap CPU
			;
	}
	DCOCTL = 1;                     // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;          // Set DCO to 1MHz //that was _1MHZ
	DCOCTL = CALDCO_1MHZ;
	/* DO NOT MODIFY */

	/*TIMERS */
	//TA0CCTL0 = CCIE; //enable interrupt of Timer 0
	//TA0CCR0 = 166; //number to which it counts up to
	//TA0CTL = TASSEL_1 + ID_3 + MC_1; //timer0: 1-which_clock; 2-divede_on_2^3; 3-configure to count up to number stored in TA0CCR0

	TA1CCR0 = 5000;                             // PWM period
	TA1CCTL1 = OUTMOD_7;        // CCR1 reset/set; (see [REF 4], Table 12-2)<br>
	TA1CCR1 = 10000;              	// CCR1 PWM duty cycle (25%)<br>
	TA1CTL = TASSEL_3 + ID_3 + MC_1; // SMCLK, divide clock by x, up mode<br> // + TAIE
	/*TIMERS */

	/*INTERRUPTS AND POWER MODES */
	__enable_interrupt(); //simply set Global Iinterrupt Enable bit to 1, same as: __bis_SR_register(GIE);
	//__bis_SR_register(GIE); //enables interrupt, if you set to 0, this bit inside interrupt means nested interrupt and priorities no longer exist, source: 2.2.3.3 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
	//__bis_SR_register(LPM0); //Low Power Mode 0: Disabled: CPU, MCLK; Enabled: ACLK, SMCLK
//	__bis_SR_register(CPUOFF+GIE);

	/*INTERRUPTS AND POWER MODES */

	/* I/O */
	P1DIR |= (LED1 + LED2); 	//configure Pin1.0 as output
	//P1DIR |= ~BTN2; //mark this as an input
	P1IE |= BTN2; //enable port interrupt
	P1IES |= BTN2; //Set the edge of interrupt ?
	P1REN |= BTN2; //Enable pullup on switch ?
	P1IFG &= ~BTN2; //Clear the interrupt ?

	P2DIR |= (BIT2 + BIT1 + BIT0); //output
	P2SEL |= (BIT2 + BIT1 + BIT0);  //output
	/* I/O */

	/* UART */ // note that, when you want to use UART interrupt, you should set jumpers in proper position, similar to: |||=
//	P1SEL |= BIT1 + BIT2;	 //Enable module function on pin
//	P1SEL2 |= BIT1 + BIT2;  //Select second module; P1.1 = RXD, P1.2=TXD
//
//	UCA0CTL1 |= UCSSEL_2;    // Set clock source SMCLK
//	UCA0BR0 = 104;                 // 1MHz 9600 (N=BRCLK/baudrate)
//	UCA0BR1 = 0;                     // 1MHz 9600
//	UCA0MCTL = UCBRS0;      // Modulation UCBRSx = 1
//	UCA0CTL1 &= ~UCSWRST;   // **Initialize USCI state machine**
////
//	IE2 |= UCA0RXIE; //interrupt from UART
//	/* UART */

	/* ADC */
//	ADC10CTL0 = SREF_1 + REFON + ADC10SHT_3 + ADC10ON; // 1.5V ref, Ref ON, 64 clocks for sample
//	ADC10CTL1 = INCH_10 + ADC10DIV_3;	// Temp Sensor is at channel X and CLK/4
//	__delay_cycles(5000);	// settle time 30ms
	/* ADC */

	/* DEFAULT VALUES */
	P1OUT &= ~(LED1 + LED2); //sets LED1 and LED2 to 0, so - when launched, it won't start with lights-on

	/* DEFAULT VALUES */
	return 0;
}

int main(void) {
 	config(); // executes function called config, that itself executes all the necesary configuration commands, so that i will not see bullshit on main function

	/* LOGIC */
//	long temp, mcuTemp;
	__delay_cycles(1000); // Wait for reference to settle

	P2OUT &= ~BIT0;
	P2OUT |= ~BIT1;

	for (;;) { //infinite loop, we can just delete this now, its not necesary, beacuse system will work anyways, since its based on interrupts.



//		ADC10CTL0 |= ENC + ADC10SC;	// Sampling and conversion start
//		while (ADC10CTL1 & BUSY)
//			; // Wait..i am converting...
//		temp = ADC10MEM;	// Read ADC memory
//		mcuTemp = ((temp - 673) * 423) / 1024;	// Convert voltage to Celsius
//		mcuTemp = ~mcuTemp;
//		mcuTemp = (ADC(temp) - CAL_ADC_25T30) * (65/(CAL_ADC_25T85 - CAL_ADC_25T30)) +30;

//		while (!(IFG2 & UCA0TXIFG))
//			;     // Wait until TX buffer is ready
//		UCA0TXBUF = mcuTemp; // Send hex value to UART, my-Tsotne ID 145852, it leaves this line when buffer is emptied = data is sent

	}
	/* LOGIC */
	return 0;
}

//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCI0RX_ISR(void) {
//
//	if (UCA0RXBUF == 0x02)
//		P1OUT |= (LED1 + LED2);
//	else
//		P1OUT &= ~(LED1 + LED2);
//}

//#pragma vector=PORT1_VECTOR
//__interrupt void Port_1(void) {
//
//	while (!(IFG2 & UCA0TXIFG))
//		;     // Wait until TX buffer is ready
//	UCA0TXBUF = 0x02; // Send hex value to UART, my-Tsotne ID 145852, it leaves this line when buffer is emptied = data is sent
//
//	P1OUT ^= (LED1 + LED2); //when button is pressed, you toggle LED1 and LED2 leds
//	P1IFG &= ~BTN2; // P1.3 IFG cleared
//	P1IES ^= BTN2; // toggle the interrupt edge
//}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A(void) {

	__delay_cycles(9000); // Wait
	P2OUT ^= BIT2;
	P1OUT ^= LED1;

	//p2.0 - EN
	//p2.1 - DIR
	//p2.2 - ST
//	  switch( TA1IV )
//	  {
//	  case TA1IV_TACCR1:			// CCR1
//		  P2OUT &= BIT0;
//	      break;
////	  case TA1IV_TACCR2:			// CCR2
////		  P2OUT &= ~BIT0;
////		  break;
//	 }

}

//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void Timer_A(void) {
//	P1OUT ^= LED1;
//}
//
//unsigned char ExampleFunction(unsigned int x) {
//	unsigned int i = 0;
//	P1OUT ^= 0x01; //toggles output P1.1.
//	while (x--) {
//		for (i = 0; i < 20000; i++)
//			//waits or does something
//			;
//	}
//	return 0;
//}
